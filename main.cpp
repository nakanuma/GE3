#include <Windows.h>
#include <cstdint>

// MyClass
#include "MyWindow.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ウィンドウの生成
	Window::Create(L"CG2WindowClass", 1280, 720);

	// ウィンドウの×ボタンが押されるまでループ
	while (!Window::ProcessMessage()) {

	}

	return 0;
}