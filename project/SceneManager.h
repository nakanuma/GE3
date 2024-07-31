#pragma once
#include "BaseScene.h"

// シーン管理
class SceneManager
{
public:
	static SceneManager* GetInstance();

	~SceneManager();

	// 次シーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 今のシーン（実行中シーン）
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;
};

