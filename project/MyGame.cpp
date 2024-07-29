#include "MyGame.h"

void MyGame::Initialize()
{
	// リークチェッカー
	D3DResourceLeakChecker::GetInstance();
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// ゲームウィンドウの生成
	window = new Window;
	window->Create(L"CG2WindowClass", 1280, 720);

	// DirectX初期化処理
	dxBase = DirectXBase::GetInstance();
	dxBase->Initialize();

	// SRVマネージャの初期化
	srvManager = SRVManager::GetInstance();
	srvManager->Initialize(dxBase);

#pragma region 汎用機能初期化
	// 入力デバイスの生成と初期化
	input = new Input();
	input->Initialize(window);

	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxBase);

	// TextureManagerの初期化
	TextureManager::Initialize(dxBase->GetDevice(), srvManager);

	// ImGuiの初期化
	ImguiWrapper::Initialize(dxBase->GetDevice(), dxBase->GetSwapChainDesc().BufferCount, dxBase->GetRtvDesc().Format, srvManager->descriptorHeap.heap_.Get());

	// ParticleManagerの生成と初期化
	particleManager = new ParticleManager;
	particleManager->Initialize(dxBase, srvManager);

	// SoundManagerの生成と初期化
	soundManager = new SoundManager;
	soundManager->Initialize();
#pragma endregion

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	///
	///	↑ ここまで3Dオブジェクトの設定
	/// 

	///
	///	↓ ここからスプライトの設定
	/// 

	///
	///	↑ ここまでスプライトの設定
	/// 

	///
	/// ↓ ここから光源の設定
	/// 

	///
	/// ↑ ここまで光源の設定
	/// 
	
	///
	/// ↓ ここからその他設定
	///
	
	// カメラのインスタンスを生成
	camera = new Camera({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f }, 0.45f);
	Camera::Set(camera); // 現在のカメラをセット

	///
	/// ↑ ここまでその他設定
	/// 
}

void MyGame::Finalize()
{
	// カメラの開放
	delete camera;

	// スプライト共通処理開放
	delete spriteCommon;
	// SoundManager開放
	delete soundManager;

	// ImGuiの終了処理
	ImguiWrapper::Finalize();
	// COMの終了処理
	CoUninitialize();
}

void MyGame::Update()
{
	// 入力の更新
	input->Update();

	// フレーム開始処理
	dxBase->BeginFrame();
	// 描画前処理
	dxBase->PreDraw();

	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager->descriptorHeap.heap_.Get() };
	dxBase->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

	// ImGuiのフレーム開始処理
	ImguiWrapper::NewFrame();

	// particleManagerの更新
	particleManager->Update();

	// 平行光源の情報の定数バッファのセット
	/*dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());*/
	// カメラの定数バッファを設定
	Camera::TransferConstantBuffer();

	ImGui::Begin("window");

	ImGui::End();

	///
	/// ↑ ここまで3Dオブジェクトの描画コマンド
	/// 

	///
	/// ↓ ここからスプライトの描画コマンド
	/// 

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon->PreDraw();

	///
	/// ↑ ここまでスプライトの描画コマンド
	/// 

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void MyGame::Draw()
{

}
