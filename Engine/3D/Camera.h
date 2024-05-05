#pragma once
#include "MyMath.h"

class Camera
{
public:
	Camera(Float3 translate, Float3 rotate = Float3(0.0f, 0.0f, 0.0f), float fov = PIf / 2.0f);
	// カメラの情報を保持
	Transform transform;
	// 視野角（ラジアン）
	float fov;
	// クリップの設定
	float nearZ = 0.1f, farZ = 1000.0f;

	Matrix MakeViewMatrix();
	Matrix MakePerspectiveFovMatrix();
};

