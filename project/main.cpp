#include "MyGame.h" 

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MyGame game;

	// ゲームの初期化
	game.Initialize();

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