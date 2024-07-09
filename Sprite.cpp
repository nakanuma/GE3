#include "Sprite.h"
#include "SpriteCommon.h"
#include "DirectXUtil.h"
#include "TextureManager.h"

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->spriteCommon = spriteCommon;



	// VertexResourceを作る
	vertexResource_ = CreateBufferResource(spriteCommon->GetDxBase()->GetDevice(), sizeof(VertexData) * 4);
	// IndexResourceを作る
	indexResource_ = CreateBufferResource(spriteCommon->GetDxBase()->GetDevice(), sizeof(uint32_t) * 6);
	// materialResourceを作る
	materialResource_ = CreateBufferResource(spriteCommon->GetDxBase()->GetDevice(), sizeof(Material));
	// TransformationResourceを作る
	transformationMatrixResource_ = CreateBufferResource(spriteCommon->GetDxBase()->GetDevice(), sizeof(TransformationMatrix));



	// VertexBufferViewを作成する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	// IndexBufferViewを作成する
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;



	// VertexResourceにデータを書き込むためのアドレスを取得してvertexDataに割り当てる
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	// materialResourceにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// materialDataの初期値を書き込む
	materialData_->color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = Matrix::Identity();

	// TransformationResourceにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::Identity();
	transformationMatrixData_->World = Matrix::Identity();



	// Transformの初期化
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f}, {0.0f, 0.0f, 0.0f} };
}

void Sprite::Update()
{
	// 頂点リソースにデータを書き込む
	// 左下
	vertexData_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	// 左上
	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	// 右下
	vertexData_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
	// 右上
	vertexData_[3].position = { 640.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };

	// インデックスリソースにデータを書き込む
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;



	// Transform情報を作る
	Matrix worldMatrix = transform_.MakeAffineMatrix();
	Matrix viewMatrix = Matrix::Identity();
	Matrix projectionMatrix = Matrix::Orthographic(static_cast<float>(Window::GetWidth()), static_cast<float>(Window::GetHeight()), 0.0f, 1000.0f);
	Matrix worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldMatrix;
}

void Sprite::Draw(uint32_t textureHandle)
{
	// VertexBufferViewを設定
	spriteCommon->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	spriteCommon->GetDxBase()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	// マテリアルCBufferの場所を設定
	spriteCommon->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// TransformatinMatrixCBufferの場所を設定
	spriteCommon->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定
	TextureManager::SetDescriptorTable(2, spriteCommon->GetDxBase()->GetCommandList(), textureHandle);
	//描画（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画
	spriteCommon->GetDxBase()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
