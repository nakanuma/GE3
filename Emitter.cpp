#include "Emitter.h"
#include "ImguiWrapper.h"
#include "Logger.h"

Emitter::Emitter()
{
}

Emitter::~Emitter()
{
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

		// パーティクルの座標を表示
		ImGui::Begin("Emitter");
		Float3 position = (*itr)->GetPosition();
		ImGui::DragFloat3("position", &position.x);
		ImGui::End();

		// DelFlagがtrueだったら
		if ((*itr)->GetDelFlag()) {
			delete* itr; // パーティクルのメモリを解放
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
	// デバッグテキストを表示
	Log("Emit\n");

	// 回転角をランダムに設定
	float rotationX = 0.0f;
	float rotationY = 0.0f;
	float rotationZ = 0.0f;
	// 移動量をランダムに設定
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	// 色をランダムに設定
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;

	// パーティクルを生成してリストへプッシュバック
	Particle* newParticle = new Particle(
		{rotationX, rotationY, rotationZ},
		{velocityX, velocityY},
		{r, g, b}
	);
	particleList_.push_back(newParticle);
}
