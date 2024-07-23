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
#include "ModelManager.h"
#include "ConstBuffer.h"
#include "Object3D.h"
#include "OutlinedObject.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "SRVManager.h"

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
	srvManager = new SRVManager;
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

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	// モデル読み込み
	ModelManager::ModelData planeModel = ModelManager::LoadObjFile("resources/Models", "plane.obj", dxBase->GetDevice());
	ModelManager::ModelData axisModel = ModelManager::LoadObjFile("resources/Models", "axis.obj", dxBase->GetDevice());

	// 平面オブジェクトの生成
	Object3D plane;
	// モデルを指定
	plane.model_ = &planeModel;
	// 初期位置を設定 
	plane.transform_.translate = { -1.6f, -1.0f, 0.0f, };
	// 初期回転角を設定
	plane.transform_.rotate = { 0.0f, 3.1f, 0.0f };

	// 2つ目の平面オブジェクトの生成
	Object3D axis;
	axis.model_ = &axisModel;
	axis.transform_.translate = { 1.6f, -1.0f, 0.0f, };
	axis.transform_.rotate = { 0.0f, 0.0f, 0.0f };

	///
	///	↑ ここまで3Dオブジェクトの設定
	/// 

	///
	///	↓ ここからスプライトの設定
	/// 

	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());
	uint32_t monsterBallGH = TextureManager::Load("resources/Images/monsterBall.png", dxBase->GetDevice());
	uint32_t checkerBoardGH = TextureManager::Load("resources/Images/checkerBoard.png", dxBase->GetDevice());
	uint32_t whiteGH = TextureManager::Load("resources/Images/white.png", dxBase->GetDevice());

	// スプライトの生成と初期化
	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, uvCheckerGH);

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
	Camera camera{ {0.0f, 0.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, 0.45f };
	Camera::Set(&camera);

	// UVTransform用の変数を用意
	Transform uvTransformSprite{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	// 選択されたブレンドモードを保存する変数
	static BlendMode selectedBlendMode = kBlendModeNormal;

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

		// 平面オブジェクトの行列更新
		plane.UpdateMatrix();
		plane.transform_.rotate.y += 0.01f;

		axis.UpdateMatrix();
		axis.transform_.rotate.z += 0.01f;

		// Spriteの更新処理
		sprite->Update();

		//// UVTransform用の行列を生成する
		//Matrix uvTransformMatrix = Matrix::Scaling(uvTransformSprite.scale);
		//uvTransformMatrix = uvTransformMatrix * Matrix::RotationZ(uvTransformSprite.rotate.z);
		//uvTransformMatrix = uvTransformMatrix * Matrix::Translation(uvTransformSprite.translate);
		//materialDataSprite->uvTransform = uvTransformMatrix;

		// キー入力でplaneを移動
		if (input->PushKey(DIK_W)) {
			plane.transform_.translate.y += 0.01f;
		}
		if (input->PushKey(DIK_S)) {
			plane.transform_.translate.y -= 0.01f;
		}

		if (input->TriggerKey(DIK_A)) {
			plane.transform_.translate.x -= 0.1f;
		}
		if (input->ReleaseKey(DIK_D)) {
			plane.transform_.translate.x += 0.1f;
		}

		// ImGui
		ImGui::Begin("Settings");
		ImGui::DragFloat3("CameraTranslate", &camera.transform.translate.x, 0.01f);
		ImGui::SliderAngle("CameraRotateX", &camera.transform.rotate.x);
		ImGui::SliderAngle("CameraRotateY", &camera.transform.rotate.y);
		ImGui::SliderAngle("CameraRotateZ", &camera.transform.rotate.z);
		ImGui::End();

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
		// 平面オブジェクトの描画
		plane.Draw();
		axis.Draw();

		///
		/// ↑ ここまで3Dオブジェクトの描画コマンド
		/// 

		///
		/// ↓ ここからスプライトの描画コマンド
		/// 

		// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
		spriteCommon->PreDraw();

		// Spriteの描画処理
		sprite->Draw();

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

	// スプライト共通処理開放
	delete spriteCommon;
	// スプライト開放
	
	// Spriteの更新処理
	delete sprite;

	// SRVManager開放
	delete srvManager;

	// ImGuiの終了処理
	ImguiWrapper::Finalize();
	// COMの終了処理
	CoUninitialize();

	return 0;
}