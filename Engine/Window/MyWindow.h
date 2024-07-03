#pragma once
#include <windows.h>
#include <cstdint>

class Window
{
public:
	// ウィンドウの作成
	static void Create(LPCWSTR windowTitle, uint32_t width, uint32_t height);
	// ウィンドウの終了を伝える
	static bool ProcessMessage();

	static uint32_t GetWidth();
	static uint32_t GetHeight();
	static HWND GetHandle();

	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	inline static uint32_t winWidth;
	inline static uint32_t winHeight;

	inline static HWND hwnd;
};

