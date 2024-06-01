#pragma once
#include "MyMath.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Object3D.h"

class Particle
{
public:
	Particle(Float3 position, Float3 rotation, Float2 velocity, Float3 color);
	~Particle();

	void Update();
	void Draw();

	bool GetDelFlag();

	// 現在の座標を取得
	Float3 GetPosition() const {
		return triangle_.transform_.translate;
	}

	//Particle(const Particle&) = delete; // コピーコンストラクターを削除
	//Particle& operator=(const Particle&) = delete; // コピー代入演算子を削除
	//Particle(Particle&&) = delete; // 移動コンストラクターを削除
	//Particle& operator=(Particle&&) = delete; // 移動代入演算子を削除
private:
	// シングルトンインスタンスの作成
	DirectXBase* dxBase = DirectXBase::GetInstance();
	// 三角形オブジェクト
	Object3D triangle_;
	// 削除フラグ
	bool del_ = false;

	// 回転に使用する数値
	Float3 rotation_;
	// 移動に使用する数値
	Float2 velocity_;
	// 色
	Float3 color_;
};

