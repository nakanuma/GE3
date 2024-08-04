#include "GamePlayScene.h"
#include "ImguiWrapper.h"
#include "DirectXBase.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

void GamePlayScene::Initialize()
{
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = new Camera({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f }, 0.45f);
	Camera::Set(camera); // 現在のカメラをセット

	// SpriteCommonの生成と初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(DirectXBase::GetInstance());

	// TextureManagerの初期化
	TextureManager::Initialize(dxBase->GetDevice(), SRVManager::GetInstance());

	// SoundManagerの初期化
	soundManager = new SoundManager();
	soundManager->Initialize();

	///
	///	↓ ゲームシーン用
	///	

	// Texture読み込み
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());

	// スプライトの生成と初期化
	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon, uvCheckerGH);
	sprite_->SetSize({ 500.0f, 500.0f });

	// モデル読み込み
	planeModel_ = ModelManager::LoadObjFile("resources/Models", "plane.obj", dxBase->GetDevice());
	sphereModel_ = ModelManager::LoadObjFile("resources/Models", "sphere.obj", dxBase->GetDevice());
	teapotModel_ = ModelManager::LoadObjFile("resources/Models", "teapot.obj", dxBase->GetDevice());
	bunnyModel_ = ModelManager::LoadObjFile("resources/Models", "bunny.obj", dxBase->GetDevice());
	multiMeshModel_ = ModelManager::LoadObjFile("resources/Models", "multiMesh.obj", dxBase->GetDevice());
	suzanneModel_ = ModelManager::LoadObjFile("resources/Models", "suzanne.obj", dxBase->GetDevice());

	// 3Dオブジェクトの生成とモデル指定
	object_ = new Object3D();
	object_->model_ = &planeModel_;
	object_->transform_.rotate = { 0.0f, 3.14f, 0.0f };

	// 音声読み込み
	soundData_ = soundManager->LoadWave("resources/Sounds/yay.wav");
}

void GamePlayScene::Finalize()
{
	// 音声データ開放
	soundManager->Unload(&soundData_);
	// SoundManager開放
	delete soundManager;

	// 3Dオブジェクト開放
	delete object_;

	// Sprite開放
	delete sprite_;

	// SpriteCommon開放
	delete spriteCommon;

	// カメラの開放
	delete camera;
}

void GamePlayScene::Update()
{
	// スプライトの更新
	sprite_->Update();

	// 3Dオブジェクトの更新
	object_->UpdateMatrix();
}

void GamePlayScene::Draw()
{
	DirectXBase* dxBase = DirectXBase::GetInstance();
	SRVManager* srvManager = SRVManager::GetInstance();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager->descriptorHeap.heap_.Get() };
	dxBase->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	// ImGuiのフレーム開始処理
	ImguiWrapper::NewFrame();
	// カメラの定数バッファを設定
	Camera::TransferConstantBuffer();

	///
	///	↓ ここから3Dオブジェクトの描画コマンド
	/// 

	// 3Dオブジェクト描画
	object_->Draw();

	///
	///	↑ ここまで3Dオブジェクトの描画コマンド
	/// 

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon->PreDraw();

	///
	/// ↓ ここからスプライトの描画コマンド
	/// 

	// スプライトの描画
	sprite_->Draw();

	///
	/// ↑ ここまでスプライトの描画コマンド
	/// 

	ImGui::Begin("window");

	// オブジェクト
	if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
		// モデル選択メニュー
		if (ImGui::Combo("Select Model", &model_current, items, IM_ARRAYSIZE(items))) {
			// 選択が変更されたときの処理
			switch (model_current) {
			case 0:
				object_->model_ = &planeModel_;
				break;
			case 1:
				object_->model_ = &sphereModel_;
				break;
			case 2:
				object_->model_ = &teapotModel_;
				break;
			case 3:
				object_->model_ = &bunnyModel_;
				break;
			case 4:
				object_->model_ = &multiMeshModel_;
				break;
			case 5:
				object_->model_ = &suzanneModel_;
				break;
			}
		}

		ImGui::DragFloat3("object.translate", &object_->transform_.translate.x, 0.01f);
		ImGui::DragFloat3("object.rotate", &object_->transform_.rotate.x, 0.01f);
		ImGui::DragFloat3("object.scale", &object_->transform_.scale.x, 0.01f);
	}

	// スプライト
	if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat2("sprite.position", &spritePosition.x, 1.0f);
		sprite_->SetPosition(spritePosition);
		ImGui::DragFloat2("sprite.size", &spriteSize.x, 1.0f);
		sprite_->SetSize(spriteSize);
		ImGui::SliderAngle("sprite.rotate", &spriteRotate);
		sprite_->SetRotation(spriteRotate);
	}

	ImGui::End();

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}
