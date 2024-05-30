#pragma once
#include "MyMath.h"
#include "TextureManager.h"
#include "Object3D.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Update();
	void Draw();

	bool GetDelFlag();

private:
	// オブジェクト
	Object3D triangle_;
	// 削除フラグ
	bool del_ = false;

	// 移動に使用する数値
	Float2 velocity_;
	// 回転に使用する数値
	Float3 rotation_;
};

