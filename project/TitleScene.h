#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Object3D.h"
#include "SoundManager.h"
#include "Input.h"

// ゲームプレイシーン
class TitleScene : public BaseScene
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
	Input* input = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	// モデルデータ
	ModelManager::ModelData model_;
	// 3Dオブジェクト
	Object3D* object_;

	// スプライト
	Sprite* sprite_;
};

