#include "Particle.h"

Particle::Particle(Float3 rotation, Float2 velocity, Float3 color)
{
	// 引数で受け取った値を設定
	rotation_ = rotation;
	velocity_ = velocity;
	color_ = color;

	// 初期位置を設定（あとでランダムに変更）
	triangle_.transform_.translate = { 0.0f, 4.0f, 0.0f };
	// 初期サイズを設定
	triangle_.transform_.scale = { 0.15f, 0.15f, 0.0f };

	// モデルを読みこむ
	static ModelData model = ModelManager::LoadObjFile("resources/Models", "triangle.obj", dxBase->GetDevice());
	// オブジェクトにモデルを設定
	triangle_.model_ = &model;
}

Particle::~Particle()
{
}

void Particle::Update()
{
	// 下に移動させる（デバッグ用）
	triangle_.transform_.translate.y -= 0.02f;

	// このラインを超えたら削除
	if (triangle_.transform_.translate.y < -4.0f) {
		del_ = true;
	}
}

void Particle::Draw()
{
	triangle_.Draw();
}

bool Particle::GetDelFlag()
{
	return del_;
}
