#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Object3D.h"
#include "SoundManager.h"

// ゲームプレイシーン
class GamePlayScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	Camera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	SoundManager* soundManager = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	// スプライト
	Sprite* sprite_;

	// モデルデータ
	ModelManager::ModelData planeModel_;
	ModelManager::ModelData sphereModel_;
	ModelManager::ModelData teapotModel_;
	ModelManager::ModelData bunnyModel_;
	ModelManager::ModelData multiMeshModel_;
	ModelManager::ModelData suzanneModel_;
	// 3Dオブジェクト
	Object3D* object_;

	// 音声データ
	SoundManager::SoundData soundData_;


	///
	///	操作用変数
	/// 
	
	// スプライト用変数
	Float2 spritePosition = { 0.0f, 0.0f };
	Float2 spriteSize = { 512.0f, 512.0f };
	float spriteRotate = 0.0f;

	// オブジェクト用変数
	const char* items[6] = { "plane", "sphere", "Teapot", "Bunny", "MultiMesh", "Suzanne" };
	int  model_current = 0;
};

