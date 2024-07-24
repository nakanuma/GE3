#include <Windows.h>    
#include <cstdint>
#include <assert.h>
#include <numbers>
#include <list>

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
#include "ModelManager.h"
#include "ConstBuffer.h"
#include "Object3D.h"
#include "OutlinedObject.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "SRVManager.h"
#include "StructuredBuffer.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"

enum BlendMode {
	kBlendModeNormal,
	kBlendModeNone,
	kBlendModeAdd,
	kBlendModeSubtract,
	kBlendModeMultiply,
	kBlendModeScreen
};

const char* BlendModeNames[6] = {
	"kBlendModeNormal",
	"kBlendModeNone",
	"kBlendModeAdd",
	"kBlendModeSubtract",
	"kBlendModeMultiply",
	"kBlendModeScreen"
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker::GetInstance();
	// 基盤機能
	Window* window = nullptr;
	DirectXBase* dxBase = nullptr;
	SRVManager* srvManager = nullptr;
	// 汎用機能
	Input* input = nullptr;
	SpriteCommon* spriteCommon = nullptr;

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
#pragma endregion

	// ParticleManagerの生成と初期化
	ParticleManager* particleManager = nullptr;
	particleManager = new ParticleManager;
	particleManager->Initialize(dxBase, srvManager);

	// ParticleEmitterの生成
	ParticleEmitter* particleEmitter = nullptr;
	particleEmitter = new ParticleEmitter(*particleManager);

	ParticleEmitter* particleEmitter2 = nullptr;
	particleEmitter2 = new ParticleEmitter(*particleManager);

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	// モデル読み込み
	ModelManager::ModelData planeModel = ModelManager::LoadObjFile("resources/Models", "plane.obj", dxBase->GetDevice());

	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());
	uint32_t circleGH = TextureManager::Load("resources/Images/circle.png", dxBase->GetDevice());

	// particleManagerにパーティクルグループを作成
	particleManager->CreateParticleGroup("particle");
	particleManager->SetModel("particle", &planeModel);
	particleManager->SetTexture("particle", circleGH);

	particleManager->CreateParticleGroup("particle2");
	particleManager->SetModel("particle2", &planeModel);
	particleManager->SetTexture("particle2", uvCheckerGH);

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

	// カメラのインスタンスを生成
	/*Camera camera{ {0.0f, 0.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, 0.45f };*/
	Camera camera{ {0.0f, 23.0f, 10.0f}, {std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f}, 0.45f };
	Camera::Set(&camera);

	// UVTransform用の変数を用意
	Transform uvTransformSprite{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	// 選択されたブレンドモードを保存する変数
	static BlendMode selectedBlendMode = kBlendModeAdd;

	// ウィンドウの×ボタンが押されるまでループ
	while (!Window::ProcessMessage()) {
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

		///
		///	更新処理
		/// 

		//////////////////////////////////////////////////////

		// particleManagerの更新
		particleManager->Update();
		// particleEmitterの更新と発生
		particleEmitter->Update("particle", true);
		particleEmitter2->Update("particle2", false);

		//////////////////////////////////////////////////////

		///
		///	描画処理
		/// 

		// 平行光源の情報の定数バッファのセット
		/*dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());*/
		// カメラの定数バッファを設定
		Camera::TransferConstantBuffer();

		///
		/// ↓ ここから3Dオブジェクトの描画コマンド
		/// 

		// 選択されたブレンドモードに変更
		switch (selectedBlendMode) {
		case kBlendModeNormal:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineState());
			break;
		case kBlendModeNone:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateBlendModeNone());
			break;
		case kBlendModeAdd:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateBlendModeAdd());
			break;
		case kBlendModeSubtract:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateBlendModeSubtract());
			break;
		case kBlendModeMultiply:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateBlendModeMultiply());
			break;
		case kBlendModeScreen:
			dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateBlendModeScreen());
			break;
		}

		ImGui::Begin("window");

		if (ImGui::Button("Add Particle")) {
			particleEmitter2->Emit("particle2");
		}

		ImGui::End();

		particleManager->Draw();

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

	///
	/// 各種開放処理
	/// 

	///
	/// ↓ここからパーティクル関連
	/// 

	// ParticleEmitterの開放
	delete particleEmitter;
	// ParticleManagerの開放
	delete particleManager;

	///
	/// ↑ここまでパーティクル関連
	/// 

	// スプライト共通処理開放
	delete spriteCommon;

	// ImGuiの終了処理
	ImguiWrapper::Finalize();
	// COMの終了処理
	CoUninitialize();

	return 0;
}