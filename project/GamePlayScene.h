#pragma once
#include "Camera.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Object3D.h"

// ゲームプレイシーン
class GamePlayScene
{
public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();

private:
	Camera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	// スプライト
	Sprite* sprite_;

	// モデル
	ModelManager::ModelData model_;
	Object3D* object_;
};

