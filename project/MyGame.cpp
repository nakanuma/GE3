#include "MyGame.h"

void MyGame::Initialize()
{
	///
	/// 基底クラスの初期化処理
	/// 
	
	Framework::Initialize();

	///
	/// ゲーム固有の初期化
	///

	// ゲームプレイシーンの生成と初期化
	scene_ = new TitleScene();
	scene_->Initialize();
	
}

void MyGame::Finalize()
{
	///
	/// ゲーム固有の終了処理
	///

	// シーンの終了と開放
	scene_->Finalize();
	delete scene_;

	///
	/// 基底クラスの終了処理
	/// 
	
	Framework::Finalize();
}

void MyGame::Update()
{
	///
	/// 基底クラスの更新処理
	/// 
	
	Framework::Update();

	///
	/// ゲーム固有の更新処理
	///
	
	// シーンの更新処理
	scene_->Update();
}

void MyGame::Draw()
{
	// シーンの描画処理
	scene_->Draw();
}
