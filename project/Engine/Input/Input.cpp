#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(Window* window)
{
	// 借りてきたwinAppのインスタンスを記録
	this->window = window;

	// DirectInputの初期化
	HRESULT result = DirectInput8Create(
		window->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	/*IDirectInputDevice8* keyboard = nullptr;*/
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		window->GetHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	HRESULT result;

	// 前回のキー入力を保存
	memcpy(keyPre, key_, sizeof(key_));

	// キーボード情報の取得開始
	result = keyboard_->Acquire();
	// 全キーの入力状態を取得する
	result = keyboard_->GetDeviceState(sizeof(key_), key_);
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーを押していればtrueを返す
	if (key_[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーが押された瞬間のみtrueを返す
	if (key_[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber)
{
	// 指定キーが離された瞬間のみtrueを返す
	if (!key_[keyNumber] && keyPre[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}
