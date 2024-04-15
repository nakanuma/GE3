#include "MyWindow.h"

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破壊された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::Create(LPCWSTR windowTitle, uint32_t width, uint32_t height)
{
	WNDCLASS wc{};

	// ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc.lpszClassName = windowTitle;
	// インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc);

	// クライアント領域のサイズ
	winWidth = width;
	winHeight = height;

	// ウィンドウサイズを表す構造体にクライアント領域をいれる
	RECT wrc = { 0, 0, static_cast<LONG>(winWidth), static_cast<LONG>(winHeight) };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd = CreateWindow(
		wc.lpszClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	// ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);

	// ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);
}

bool Window::ProcessMessage()
{
	MSG msg{};

	// ウィンドウの×ボタンが押されるまでループ
	// Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message == WM_QUIT;
}

uint32_t Window::GetWidth()
{
	return winWidth;
}

uint32_t Window::GetHeight()
{
	return winHeight;
}

HWND Window::GetHandle()
{
	return hwnd;
}
