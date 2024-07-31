#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSON 0x0800 // DirectInputのバージョン指定
#include <dinput.h>

#include "MyWindow.h"

// 入力
class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Window* window);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（ DIK_0等 ）</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（ DIK_0等 ）</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーのリリースをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（ DIK_0等 ）</param>
	/// <returns>離されたか</returns>
	bool ReleaseKey(BYTE keyNumber);

private: // メンバ変数
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput_;

	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard_;

	// 全キーの状態
	BYTE key_[256] = {};
	
	// 前回の全キーの状態
	BYTE keyPre[256] = {};

	// WindowsAPI
	Window* window = nullptr;
};

