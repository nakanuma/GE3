#include <Windows.h>
#include <cstdint>
#include <assert.h>

// MyClass
#include "MyWindow.h"
#include "Logger.h"
#include "StringUtil.h"
#include "DirectXBase.h"
#include "DirectXUtil.h"
#include "MyMath.h"
#include "Camera.h"
#include "DescriptorHeap.h"
#include "ImguiWrapper.h"
#include "TextureManager.h"

struct VertexData {
	Float4 position;
	Float2 texcoord;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DirectXBase* dxBase = nullptr;

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// ウィンドウの生成
	Window::Create(L"CG2WindowClass", 1280, 720);

	// DirectX初期化処理
	dxBase = DirectXBase::GetInstance();
	dxBase->Initialize();

	// TextureManagerの初期化（srvHeapの生成）
	TextureManager::Initialize(dxBase->GetDevice().Get());

	// ImGuiの初期化
	ImguiWrapper::Initialize(dxBase->GetDevice().Get(), dxBase->GetSwapChainDesc().BufferCount, dxBase->GetRtvDesc().Format, TextureManager::GetInstance().srvHeap_.heap_.Get());

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	// 球体の設定を行う
	const uint32_t kSubdivision = 20; // 分割数
	const float kLonEvery = (PIf * 2.0f) / float(kSubdivision); // 経度分割1つ分の角度 φ
	const float kLatEvery = PIf / float(kSubdivision); // 緯度分割1つ分の角度 θ
	// 必要な頂点数を計算
	const uint32_t vertexNum = kSubdivision * kSubdivision * 6;

	// 頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(VertexData) * vertexNum);

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexNum;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));


	// 経度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -PIf / 2.0f + kLatEvery * latIndex; // 現在の緯度
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery; // 現在の経度
			// 頂点にデータを入力する
			// 1枚目の三角形
			// 左下（基準点a）
			vertexData[start].position.x = cosf(lat) * cosf(lon);
			vertexData[start].position.y = sinf(lat);
			vertexData[start].position.z = cosf(lat) * sinf(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			// 上（基準点b）
			vertexData[start + 1].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData[start + 1].position.y = sinf(lat + kLatEvery);
			vertexData[start + 1].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[start + 1].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			// 右下（基準点c）
			vertexData[start + 2].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData[start + 2].position.y = sinf(lat);
			vertexData[start + 2].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData[start + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			// 2枚目の三角形
			// 左上（基準点b）
			vertexData[start + 3].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData[start + 3].position.y = sinf(lat + kLatEvery);
			vertexData[start + 3].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData[start + 3].position.w = 1.0f;
			vertexData[start + 3].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[start + 3].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			// 右上（基準点d）
			vertexData[start + 4].position.x = cosf(lat + kLatEvery) * cosf(lon + kLonEvery);
			vertexData[start + 4].position.y = sinf(lat + kLatEvery);
			vertexData[start + 4].position.z = cosf(lat + kLatEvery) * sinf(lon + kLonEvery);
			vertexData[start + 4].position.w = 1.0f;
			vertexData[start + 4].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData[start + 4].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			// 右下（基準点c）
			vertexData[start + 5].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData[start + 5].position.y = sinf(lat);
			vertexData[start + 5].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData[start + 5].position.w = 1.0f;
			vertexData[start + 5].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData[start + 5].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
		}
	}

	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(Float4));
	// マテリアルにデータを書き込む
	Float4* materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 白を書き込む
	*materialData = Float4(1.0f, 1.0f, 1.0f, 1.0f);


	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(Matrix));
	// データを書き込む
	Matrix* wvpData = nullptr;
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	// 単位行列を書き込んでおく
	*wvpData = Matrix::Identity();

	// Transform変数を作る
	Transform transform{ {1.0f,1.0f,1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	///
	///	↑ ここまで3Dオブジェクトの設定
	/// 

	///
	///	↓ ここからスプライトの設定
	/// 

	// Sprite用のリソースを作る
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResourceSprite = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(VertexData) * 6);

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	// リソースの先頭のアドレスから使う
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点4つ分のサイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);

	// 頂点データを設定
	VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	// 1枚目の三角形
	// 左下
	vertexDataSprite[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };
	vertexDataSprite[0].texcoord = { 0.0f, 1.0f };
	// 左上
	vertexDataSprite[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexDataSprite[1].texcoord = { 0.0f, 0.0f };
	// 右下
	vertexDataSprite[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };
	vertexDataSprite[2].texcoord = { 1.0f, 1.0f };
	// 2枚目の三角形
	// 左上
	vertexDataSprite[3].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexDataSprite[3].texcoord = { 0.0f, 0.0f };
	// 右上
	vertexDataSprite[4].position = { 640.0f, 0.0f, 0.0f, 1.0f };
	vertexDataSprite[4].texcoord = { 1.0f, 0.0f };
	// 右下
	vertexDataSprite[5].position = { 640.0f, 360.0f, 0.0f, 1.0f };
	vertexDataSprite[5].texcoord = { 1.0f, 1.0f };

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResourceSprite = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(Matrix));
	// データを書き込む
	Matrix* transformationMatrixDataSprite = nullptr;
	// 書き込むためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	// 単位行列を書き込んでおく
	*transformationMatrixDataSprite = Matrix::Identity();

	// CPUで動かす用のTransformを作る
	Transform transformSprite{ {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f}, {0.0f, 0.0f, 0.0f} };

	///
	///	↑ ここまでスプライトの設定
	/// 

	// カメラのインスタンスを生成
	Camera camera{ {0.0f, 0.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, 0.45f };

	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/uvChecker.png", dxBase->GetDevice().Get());

	// ウィンドウの×ボタンが押されるまでループ
	while (!Window::ProcessMessage()) {
		// フレーム開始処理
		dxBase->BeginFrame();
		// 描画前処理
		dxBase->PreDraw();

		// 描画用のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { TextureManager::GetInstance().srvHeap_.heap_.Get() };
		dxBase->GetCommandList().Get()->SetDescriptorHeaps(1, descriptorHeaps);

		// ImGuiのフレーム開始処理
		ImguiWrapper::NewFrame();

		///
		///	更新処理
		/// 

		//////////////////////////////////////////////////////

		// 球体の頂点情報を更新
		transform.rotate.y += 0.03f;

		Matrix worldMatrix = transform.MakeAffineMatrix();
		Matrix viewMatrix = camera.MakeViewMatrix();
		Matrix projectionMatrix = camera.MakePerspectiveFovMatrix();
		Matrix worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		*wvpData = worldViewProjectionMatrix;


		// Sprite用のWorldViewProjectionMatrixを作る
		Matrix worldMatrixSprite = transformSprite.MakeAffineMatrix();
		Matrix viewMatrixSprite = Matrix::Identity();
		Matrix projectionMatrixSprite = Matrix::Orthographic(static_cast<float>(Window::GetWidth()), static_cast<float>(Window::GetHeight()), 0.0f, 1000.0f);
		Matrix worldViewProjectionMatrixSprite = worldMatrixSprite * viewMatrixSprite * projectionMatrixSprite;
		*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;


		// ImGui
		ImGui::Begin("Settings");
		// マテリアルの色を変更できるようにする
		ImGui::ColorEdit4("MaterialColor", &materialData->x);
		// スプライトの位置を変更できるようにする
		ImGui::DragFloat3("SpritePosition", &transformSprite.translate.x);
		// カメラの操作を行えるようにする
		ImGui::DragFloat3("CameraTranslate", &camera.transform.translate.x);
		ImGui::End();

		//////////////////////////////////////////////////////

		///
		///	描画処理
		/// 

		///
		/// ↓ ここから3Dオブジェクトの描画コマンド
		/// 

		// commandListにVBVを設定
		dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
		// マテリアルCBufferの場所を設定
		dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		// wvp用のCBufferの場所を設定
		dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		// SRVのDescriptorTableの先頭を設定（Textureの設定）
		TextureManager::SetDescriptorTable(2, dxBase->GetCommandList().Get(), uvCheckerGH);
		// 描画を行う（DrawCall/ドローコール）
		dxBase->GetCommandList()->DrawInstanced(vertexNum, 1, 0, 0);

		///
		/// ↑ ここまで3Dオブジェクトの描画コマンド
		/// 

		///
		/// ↓ ここからスプライトの描画コマンド
		/// 

		// VBVを設定
		dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
		// TransformatinMatrixCBufferの場所を設定
		dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
		// 描画
		dxBase->GetCommandList()->DrawInstanced(6, 1, 0, 0);

		///
		/// ↑ ここまでスプライトの描画コマンド
		/// 

		// ImGuiの内部コマンドを生成する
		ImguiWrapper::Render(dxBase->GetCommandList().Get());
		// 描画後処理
		dxBase->PostDraw();
		// フレーム終了処理
		dxBase->EndFrame();
	}
	// ImGuiの終了処理
	ImguiWrapper::Finalize();

	// COMの終了処理
	CoUninitialize();

	return 0;
}