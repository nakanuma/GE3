#include "ModelManager.h"
#include <fstream>
#include <sstream>
#include <DirectXUtil.h>
#include <DirectXBase.h>

ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename, ID3D12Device* device)
{
    // 1. 中で必要となる変数の宣言
    ModelData modelData; // 構築するModelData
    std::vector<Float4> positions; // 位置
    std::vector<Float3> normals; // 法線
    std::vector<Float2> texcoords; // テクスチャ座標
    std::string line; // ファイルから読んだ1行を格納するもの

    // 2. ファイルを開く
    std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
    assert(file.is_open()); // とりあえず開けなかったら止める

    // 3. 実際にファイルを読み、ModelDataを構築していく
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier; // 先頭の識別子を読む

        // identifierに応じた処理
        if (identifier == "v") {
            Float4 position;
            s >> position.x >> position.y >> position.z;
            position.x *= -1.0f; // 位置のxを反転
            position.w = 1.0f;
            positions.push_back(position);
        } else if (identifier == "vt") {
            Float2 texcoord;
            s >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y; // テクスチャが上下反転しないようにする
            texcoords.push_back(texcoord);
        } else if (identifier == "vn") {
            Float3 normal;
            s >> normal.x >> normal.y >> normal.z;
            normal.x *= -1.0f; // 法線のxを反転
            normals.push_back(normal);
        } else if (identifier == "f") {
            VertexData triangle[3];
            // 面は三角形限定。その他は未対応
            for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
                std::string vertexDefinition;
                s >> vertexDefinition;
                // 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
                std::istringstream v(vertexDefinition);
                uint32_t elementIndices[3];
                for (int32_t element = 0; element < 3; ++element) {
                    std::string index;
                    std::getline(v, index, '/'); // /区切りでインデックスを読んでいく
                    elementIndices[element] = std::stoi(index);
                }
                // 要素へのIndexから、実際の要素の値を取得して、頂点を構成する
                Float4 position = positions[elementIndices[0] - 1];
                Float2 texcoord = texcoords[elementIndices[1] - 1];
                Float3 normal = normals[elementIndices[2] - 1];
                triangle[faceVertex] = { position, texcoord, normal };
            }
            // 頂点を逆順に登録することで、回り順を逆にする
            modelData.vertices.push_back(triangle[2]);
            modelData.vertices.push_back(triangle[1]);
            modelData.vertices.push_back(triangle[0]);
        } else if (identifier == "mtllib") {
            // materialTemplateLibraryファイルの名前を取得する
            std::string materialFilename;
            s >> materialFilename;
            // 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
            modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename, device);
        }
    }

    // vertexResourceの作成
    modelData.vertexResource = CreateBufferResource(DirectXBase::GetInstance()->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

    // 頂点バッファビューを作成する
    modelData.vertexBufferView;
    // リソースの先頭のアドレスから使う
    modelData.vertexBufferView.BufferLocation = modelData.vertexResource->GetGPUVirtualAddress();
    // 使用するリソースのサイズは頂点のサイズ
    modelData.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
    // 1頂点あたりのサイズ
    modelData.vertexBufferView.StrideInBytes = sizeof(VertexData);


    // 頂点リソースにデータを書き込む
    VertexData* vertexData = nullptr;
    // 書き込むためのアドレスを取得
    modelData.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    // 頂点データをリソースにコピー
    std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

    // 4. ModelDataを返す
    return modelData;
}

MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename, ID3D12Device* device)
{
    // 1. 中で必要となる変数の宣言
    MaterialData materialData; // 構築するMaterialData
    std::string line; // ファイルから読んだ1行を格納するもの

    // 2. ファイルを開く
    std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
    assert(file.is_open()); // とりあえず開けなかったら止める

    // 3. 実際にファイルを読み、MaterialDataを構築していく
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;

        // identifierに応じた処理
        if (identifier == "map_Kd") {
            std::string textureFilename;
            s >> textureFilename;
            // 連結してファイルパスにする
            materialData.textureFilePath = directoryPath + "/" + textureFilename;
            // 画像を読み込む
            materialData.textureHandle = TextureManager::Load(materialData.textureFilePath, device);
        }
    }

    // 4. MaterialDataを返す
    return materialData;
}
