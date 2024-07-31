#pragma once
#include "Camera.h"
#include "SpriteCommon.h"

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
	
};

