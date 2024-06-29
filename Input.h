#pragma once
#include <Windows.h>

/// <summary>
/// 入力
/// </summary>
class Input
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
};

