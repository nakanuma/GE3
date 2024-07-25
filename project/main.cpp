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
#include "SoundManager.h"

/*--------------------サウンド関連ここから--------------------*/

//// チャンクヘッダ
//struct ChunkHeader {
//	char id[4]; // チャンク毎のID
//	int32_t size; // チャンクサイズ
//};
//
//// RIFFヘッダチャンク
//struct RiffHeader {
//	ChunkHeader chunk; // "RIFF"
//	char type[4]; // "WAVE"
//};
//
//// FMTチャンク
//struct FormatChunk {
//	ChunkHeader chunk; // "fmt"
//	WAVEFORMATEX fmt; // 波形フォーマット
//};
//
//// 音声データ
//struct SoundData {
//	// 波形フォーマット
//	WAVEFORMATEX wfex;
//	// バッファの先頭アドレス
//	BYTE* pBuffer;
//	// バッファのサイズ
//	unsigned int bufferSize;
//};

//SoundData SoundLoadWave(const char* filename) {
//	/*HRESULT result;*/
//
//	///
//	/// 1, ファイルオープン
//	///
//
//	// ファイル入力ストリームのインスタンス
//	std::ifstream file;
//	// .wavファイルをバイナリモードで開く
//	file.open(filename, std::ios_base::binary);
//	// ファイルオープン失敗を検出する
//	assert(file.is_open());
//
//	///
//	/// 2, .wavデータ読み込み
//	///
//
//	// RIFFヘッダーの読み込み
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	// ファイルがRIFFかチェック
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
//		assert(0);
//	}
//	// タイプがWAVEかチェック
//	if (strncmp(riff.type, "WAVE", 4) != 0) {
//		assert(0);
//	}
//
//	// Formatチャンクの読み込み
//	FormatChunk format = {};
//	// チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
//		assert(0);
//	}
//
//	// チャンク本体の読み込み
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//
//	// Dataチャンクの読み込み
//	ChunkHeader data;
//	file.read((char*)&data, sizeof(data));
//	// JUNKチャンクを検出した場合
//	if (strncmp(data.id, "JUNK", 4) == 0) {
//		// 読み取り位置をJUNKチャンクの終わりまで進める
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//
//	// 不要なチャンクをスキップするループ
//	while (strncmp(data.id, "data", 4) != 0) {
//		file.seekg(data.size, std::ios_base::cur);
//		file.read((char*)&data, sizeof(data));
//	}
//
//	if (strncmp(data.id, "data", 4) != 0) {
//		assert(0);
//	}
//
//	// Dataチャンクのデータ部（波形データ）の読み込み
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//	///
//	/// 3, ファイルクローズ
//	///
//
//	// Waveファイルを閉じる
//	file.close();
//
//	///
//	/// 4, 読み込んだ音声データをreturn
//	///
//
//	// returnする為の音声データ
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}

// 音声データ開放
//void SoundUnLoad(SoundData* soundData) {
//	// バッファのメモリを開放
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}

//// 音声再生
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {
//	HRESULT result;
//
//	// 波形フォーマットを元にSourceVoiceの生成
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	// 再生する波形データの設定
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	// 波形データの再生
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//}

/*--------------------サウンド関連ここまで--------------------*/

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

	/*--------------------サウンド関連ここから--------------------*/

	//// 変数の宣言
	//Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	//IXAudio2MasteringVoice* masterVoice;

	//HRESULT result;
	//// XAudioエンジンのインスタンスを生成
	//result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//// マスターボイスを生成
	//result = xAudio2->CreateMasteringVoice(&masterVoice);

	// SoundManagerの生成と初期化
	SoundManager* soundManager = nullptr;
	soundManager = new SoundManager;
	soundManager->Initialize();

	// 音声読み込み
	SoundManager::SoundData soundData1 = soundManager->LoadWave("Engine/Sound/yay.wav");

	/*--------------------サウンド関連ここまで--------------------*/

	///
	///	↓ ここから3Dオブジェクトの設定
	/// 

	// モデル読み込み
	ModelManager::ModelData planeModel = ModelManager::LoadObjFile("resources/Models", "plane.obj", dxBase->GetDevice());

	// Textureを読み込む
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());
	uint32_t circleGH = TextureManager::Load("resources/Images/circle.png", dxBase->GetDevice());

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

		// ボタンが押されたときに音声を再生
		if (ImGui::Button("PlaySound")) {
			soundManager->PlayWave(soundData1);
		}

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

	///
	/// 各種開放処理
	/// 

	///
	/// ↓ここからパーティクル関連
	/// 

	///
	/// ↑ここまでパーティクル関連
	/// 

	// スプライト共通処理開放
	delete spriteCommon;
	// 音声データ開放
	soundManager->Unload(&soundData1);
	// SoundManager開放
	delete soundManager;

	// ImGuiの終了処理
	ImguiWrapper::Finalize();
	// COMの終了処理
	CoUninitialize();

	return 0;
}