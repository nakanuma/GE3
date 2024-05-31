#pragma once
#include "Particle.h"
#include <list>
#include <iostream>
#include <unordered_map>

class Emitter
{
public:
	Emitter();
	~Emitter();

	void Initialize();
	void Update();
	void Draw();
	void Emit();
private:
	// パーティクルのリスト
	std::list<std::unique_ptr<Particle>> particleList_;

	// パーティクル発生のタイマー
	uint32_t timer_ = 0;
	// 発生間隔
	uint32_t interval_ = 20;

	// 色の配列を定義
	Float3 colors[7] = {
		{Float3{1.0, 0.0, 0.0}},     // 赤
		{Float3{1.0, 1.0, 0.0}},     // 黄
		{Float3{1.0, 0.5, 0.0}},     // オレンジ
		{Float3{0.5, 0.0, 0.5}},     // 紫
		{Float3{0.0, 0.0, 1.0}},     // 青
		{Float3{0.0, 1.0, 1.0}},     // 水色
		{Float3{0.5, 1.0, 0.0}}      // 黄緑
	};
};

