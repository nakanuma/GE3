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

	//////////////////////////////////////////////////////

	// 頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(dxBase->GetDevice().Get(), sizeof(VertexData) * 6);

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	// 左下
	vertexData[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 1.0f };
	// 上
	vertexData[1].position = { 0.0f, 0.5f, 0.0f, 1.0f };
	vertexData[1].texcoord = { 0.5f, 0.0f };
	// 右下
	vertexData[2].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };

	// 2枚目の三角形
	// 左下2
	vertexData[3].position = { -0.5f, -0.5f, 0.5f, 1.0f };
	vertexData[3].texcoord = { 0.0f, 1.0f };
	// 上2
	vertexData[4].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData[4].texcoord = { 0.5f, 0.0f };
	// 右下2
	vertexData[5].position = { 0.5f, -0.5f, -0.5f, 1.0f };
	vertexData[5].texcoord = { 1.0f, 1.0f };

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
	// カメラのインスタンスを生成
	Camera camera{ {0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, 0.45f };


	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/uvChecker.png", dxBase->GetDevice().Get());

	//////////////////////////////////////////////////////

	// ウィンドウの×ボタンが押されるまでループ
	while (!Window::ProcessMessage()) {
		// フレーム開始処理
		dxBase->BeginFrame();
		// 描画前処理
		dxBase->PreDraw();

		// 描画用のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { TextureManager::GetInstance().srvHeap_.heap_.Get()};
		dxBase->GetCommandList().Get()->SetDescriptorHeaps(1, descriptorHeaps);

		// ImGuiのフレーム開始処理
		ImguiWrapper::NewFrame();

		///
		///	更新処理
		/// 

		//////////////////////////////////////////////////////

		// 三角形の頂点情報を更新
		transform.rotate.y += 0.03f;

		Matrix worldMatrix = transform.MakeAffineMatrix();
		Matrix viewMatrix = camera.MakeViewMatrix();
		Matrix projectionMatrix = camera.MakePerspectiveFovMatrix();
		Matrix worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		*wvpData = worldViewProjectionMatrix;

		// ImGuiのUIを表示
		/*ImGui::ShowDemoWindow();*/

		// 三角形の色を変更できるようにする
		ImGui::Begin("Triangle");
		ImGui::ColorEdit4("TriangleColor", &materialData->x);
		ImGui::End();

		//////////////////////////////////////////////////////

		///
		///	描画処理
		/// 

		//////////////////////////////////////////////////////

		// commandListにVBVを設定
		dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
		// マテリアルCBufferの場所を設定
		dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		// wvp用のCBufferの場所を設定
		dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		// SRVのDescriptorTableの先頭を設定（Textureの設定）
		TextureManager::SetDescriptorTable(2, dxBase->GetCommandList().Get(), uvCheckerGH);

		// 描画を行う（DrawCall/ドローコール）
		dxBase->GetCommandList()->DrawInstanced(6, 1, 0, 0);

		//////////////////////////////////////////////////////

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