#include "Particle.h"
#include "ImguiWrapper.h"

Particle::Particle(Float3 position, Float3 rotation, Float2 velocity, Float3 color)
{
	// 引数で受け取った値を設定（移動と回転に使用する数値）
	rotation_ = rotation;
	velocity_ = velocity;

	// 引数で受け取った初期位置を設定
	triangle_.transform_.translate = position;
	// 初期サイズを設定
	triangle_.transform_.scale = { 0.15f, 0.15f, 0.0f };
	// ライティングしないように設定
	triangle_.materialCB_.data_->enableLighting = false;
	// 引数で受け取った色に設定
	triangle_.materialCB_.data_->color = { color.x, color.y, color.z, 1.0f };

	// モデル読み込み
	static ModelData model = ModelManager::LoadObjFile("resources/Models", "triangle.obj", dxBase->GetDevice());
	// オブジェクトにモデルを設定
	triangle_.model_ = &model;
}

Particle::~Particle()
{
}

void Particle::Update()
{
	// 移動
	triangle_.transform_.translate.x += velocity_.x;
	triangle_.transform_.translate.y += velocity_.y;

	// 回転
	triangle_.transform_.rotate.x += rotation_.x;
	triangle_.transform_.rotate.y += rotation_.y;
	triangle_.transform_.rotate.z += rotation_.z;

	// 行列の転送
	triangle_.UpdateMatrix();

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
