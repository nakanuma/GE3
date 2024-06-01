#include "Emitter.h"
#include "ImguiWrapper.h"
#include "Logger.h"
#include "Util.h"

Emitter::Emitter()
{
}

Emitter::~Emitter()
{
}

void Emitter::Initialize()
{
	particleList_.clear();
}

void Emitter::Update()
{
	// タイマーを増加させる
	timer_++;
	// 発生間隔毎に生成
	if (timer_ % interval_ == 0) {
		Emitter::Emit();
	}

	// パーティクル全ての更新処理を呼ぶ
	for (auto itr = particleList_.begin(); itr != particleList_.end();) {
		(*itr)->Update(); // メソッド呼び出しに->演算子を使用

		//// パーティクルの座標を表示
		//ImGui::Begin("Emitter");
		//Float3 position = (*itr)->GetPosition();
		//ImGui::DragFloat3("position", &position.x);
		//ImGui::End();

		// DelFlagがtrueだったら
		if ((*itr)->GetDelFlag()) {
			itr = particleList_.erase(itr);
		} else {
			itr++; // eraseしなかった場合のみイテレーターをインクリメント
		}
	}
}

void Emitter::Draw()
{
	// パーティクル全ての描画処理を呼ぶ
	for (auto& particle : particleList_) {
		particle->Draw();
	}
}

void Emitter::Emit()
{
	// 初期位置をランダムに設定
	float positionX = RandomNumber(-3.5f, 3.5f);
	float positionY = 4.0f;
	// 回転角をランダムに設定
	float rotationX = RandomNumber(0.00f, 0.03f);
	float rotationY = RandomNumber(0.02f, 0.03f);
	float rotationZ = RandomNumber(0.02f, 0.04f);
	// 移動量をランダムに設定
	float velocityX = RandomNumber(-0.005f, 0.005f);
	float velocityY = RandomNumber(-0.025f, -0.015f);
	// 色の決定に使用する数字をランダムに生成
	int colorNumber = RandomNumber(0, 6);

	// パーティクルを生成してリストへユニークポインタをプッシュバック
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>(
		Float3{positionX, positionY, 0.0f},
		Float3{ rotationX, rotationY, rotationZ },
		Float2{ velocityX, velocityY },
		Float3{ colors[colorNumber] }
	);
	particleList_.push_back(std::move(newParticle));
}
