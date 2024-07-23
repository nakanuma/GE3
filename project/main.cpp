#include <Windows.h>    
#include <cstdint>
#include <assert.h>
#include <random>
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

struct Particle {
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;
};

// パーティクルの生成関数
Particle MakeNewParticle(std::mt19937& randomEngine, const Float3& translate) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	Particle particle;
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	particle.transform.translate = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };

	Float3 randomTranslate{ distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.transform.translate = translate + randomTranslate;

	// 色をランダムに初期化
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine), distColor(randomEngine), distColor(randomEngine), 1.0f };

	// 生存可能時間と経過時間を初期化
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;

	return particle;
}

struct Emitter {
	Transform transform; //!< エミッタのTransform
	uint32_t count; //!< 発生数
	float frequency; //!< 発生頻度
	float frequencyTime; // !< 頻度用時刻
};

std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine) {
	std::list<Particle> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translate));
	}
	return particles;
}

// Field
struct AccelerationField {
	Float3 acceleration; //!< 加速度
	AABB area; //!< 範囲
};

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

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	// モデル読み込み
	ModelManager::ModelData planeModel = ModelManager::LoadObjFile("resources/Models", "plane.obj", dxBase->GetDevice());

	// 平面オブジェクトの生成
	Object3D plane;
	// モデルを指定
	plane.model_ = &planeModel;
	// 初期回転角を設定
	plane.transform_.rotate = { 0.0f, 3.1f, 0.0f };



	/*パーティクル設定*/

	// StructuredBufferの作成
	StructuredBuffer<Object3D::ParticleForGPU> instancingBuffer(100); // 最大インスタンス数を設定

	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < instancingBuffer.numMaxInstance_; ++index) {
		instancingBuffer.data_[index].WVP = Matrix::Identity();
		instancingBuffer.data_[index].World = Matrix::Identity();
		instancingBuffer.data_[index].color = Float4(1.0f, 1.0f, 1.0f, 1.0f); // とりあえず白を書き込む
	}

	// Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	/*const float kDeltaTime = 1.0f / 240.0f;*/

	// 乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 反対側に回す回転行列
	Matrix backToFrontMatrix = Matrix::RotationY(std::numbers::pi_v<float>);
	// billboard行列
	Matrix billboardMatrix;
	// billboardを適用するかどうか
	bool useBillBoard = true;
	// Updateを行うかどうか
	bool isParticleUpdate = true;

	// Emitter
	Emitter emitter{};
	emitter.count = 3;
	emitter.frequency = 0.5f; // 0.5秒ごとに発生
	emitter.frequencyTime = 0.0f; // 発生頻度用の時刻、0で初期化
	emitter.transform.translate = { 0.0f, 0.0f, 0.0f };
	emitter.transform.rotate = { 0.0f, 0.0f, 0.0f };
	emitter.transform.scale = { 1.0f, 1.0f, 1.0f };

	// Field
	AccelerationField accelerationField;
	accelerationField.acceleration = { 15.0f, 0.0f, 0.0f };
	accelerationField.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationField.area.max = { 1.0f, 1.0f, 1.0f };

	// パーティクルのリスト
	std::list<Particle> particles;

	///
	///	↑ ここまで3Dオブジェクトの設定
	/// 

	///
	///	↓ ここからスプライトの設定
	/// 

	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());
	uint32_t circleGH = TextureManager::Load("resources/Images/circle.png", dxBase->GetDevice());

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

		//// UVTransform用の行列を生成する
		//Matrix uvTransformMatrix = Matrix::Scaling(uvTransformSprite.scale);
		//uvTransformMatrix = uvTransformMatrix * Matrix::RotationZ(uvTransformSprite.rotate.z);
		//uvTransformMatrix = uvTransformMatrix * Matrix::Translation(uvTransformSprite.translate);
		//materialDataSprite->uvTransform = uvTransformMatrix;

		// 平面オブジェクトの行列更新
		plane.UpdateMatrix();

		Matrix cameraMatrix = Camera::GetCurrent()->MakeViewMatrix();
		if (useBillBoard) {
			billboardMatrix = backToFrontMatrix * cameraMatrix;
			billboardMatrix.r[3][0] = 0.0f;
			billboardMatrix.r[3][1] = 0.0f;
			billboardMatrix.r[3][2] = 0.0f;
		} else {
			billboardMatrix = Matrix::Identity();
		}

		// パーティクルの更新
		uint32_t numInstance = 0; // 描画すべきインスタンス数
		for (std::list<Particle>::iterator particleIterator = particles.begin();
			particleIterator != particles.end();) {
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = particles.erase(particleIterator); // 生存期間が過ぎたParticleはlistから消す。戻り値が次のイテレーターとなる
				continue;
			}
			Matrix worldMatrix = particleIterator->transform.MakeAffineMatrix();
			Matrix viewMatrix = Camera::GetCurrent()->MakeViewMatrix();
			Matrix projectionMatrix = Camera::GetCurrent()->MakePerspectiveFovMatrix();
			Matrix viewProjectionMatrix = viewMatrix * projectionMatrix;
			/*Matrix worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;*/

			// パーティクルのビルボード行列を適用
			Matrix worldViewProjectionMatrix = worldMatrix * billboardMatrix * viewProjectionMatrix;

			instancingBuffer.data_[numInstance].WVP = worldViewProjectionMatrix;
			instancingBuffer.data_[numInstance].World = worldMatrix;
			instancingBuffer.data_[numInstance].color = particleIterator->color; // パーティクルの色をそのままコピー

			if (numInstance < instancingBuffer.numMaxInstance_) { // パーティクルのインスタンス数がバッファのサイズを超えないようにする
				instancingBuffer.data_[numInstance].WVP = worldViewProjectionMatrix;
				instancingBuffer.data_[numInstance].World = worldMatrix;
				instancingBuffer.data_[numInstance].color = particleIterator->color; // パーティクルの色をそのままコピー
			}

			// Fieldの範囲内のParticleには加速度を適用する
			if (isParticleUpdate) {
				if (IsCollision(accelerationField.area, particleIterator->transform.translate)) {
					particleIterator->velocity += accelerationField.acceleration * kDeltaTime;
				}
			}

			particleIterator->transform.translate += particleIterator->velocity * kDeltaTime;
			particleIterator->currentTime += kDeltaTime; // 経過時間を足す

			float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime); // 経過時間に応じたAlpha値を算出
			instancingBuffer.data_[numInstance].color.w = alpha; // GPUに送る

			++numInstance; // 生きているParticleの数を1つカウントする

			++particleIterator; // 次のイテレータに進める
		}

		// Emitterの更新を行い、経過時間によってParticleを発生させる
		emitter.frequencyTime += kDeltaTime; // 時刻を進める
		if (emitter.frequency <= emitter.frequencyTime) { // 発生時刻より大きいなら発生
			particles.splice(particles.end(), Emit(emitter, randomEngine)); // 発生処理
			emitter.frequencyTime -= emitter.frequency; // 余計に過ぎた時間も加味して頻度計算する
		}

		// ImGui
		ImGui::Begin("Settings");
		ImGui::DragFloat3("camera.translate", &camera.transform.translate.x, 0.01f);
		ImGui::DragFloat3("camera.rotate", &camera.transform.rotate.x, 0.01f);
		ImGui::Checkbox("update", &isParticleUpdate);
		ImGui::Checkbox("useBillboard", &useBillBoard);
		if (ImGui::Button("Add Particle")) {
			particles.splice(particles.end(), Emit(emitter, randomEngine));
		}
		ImGui::DragFloat3("EmitterTranslate", &emitter.transform.translate.x, 0.01f, -100.0f, 100.0f);
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
		plane.DrawInstancing(instancingBuffer, numInstance, circleGH);

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

	// ParticleManagerの開放
	delete particleManager;
	// スプライト共通処理開放
	delete spriteCommon;
	// SRVManager開放
	delete srvManager;

	// ImGuiの終了処理
	ImguiWrapper::Finalize();
	// COMの終了処理
	CoUninitialize();

	return 0;
}