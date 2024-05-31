#pragma once
#include "Particle.h"
#include <list>

class Emitter
{
public:
	Emitter();
	~Emitter();

	void Update();
	void Draw();
	void Emit();
private:
	// パーティクルのリスト
	std::list<Particle*> particleList_;

	// パーティクル発生のタイマー
	uint32_t timer_ = 0;
	// 発生間隔
	uint32_t interval_ = 60;
};

