#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MyGame game;

	// ゲームの初期化
	game.Initialize();

	// カメラのインスタンスを生成
	Camera camera{ {0.0f, 0.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, 0.45f };
	Camera::Set(&camera);

	// ウィンドウの×ボタンが押されるまでループ
	while (!Window::ProcessMessage()) {
		// 毎フレーム更新
		game.Update();

		// 描画
		game.Draw();
	}

	game.Finalize();

	return 0;
}