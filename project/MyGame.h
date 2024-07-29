#pragma once
#include <Windows.h> 
#include <cstdint>
#include <assert.h>
#include <numbers>
#include <list>

// MyClass 
#include "MyWindow.h"
#include "Logger.h"
#include "StringUtil.h"
#include "DirectXBase.h"
#include "DirectXUtil.h"
#include "MyMath.h"
#include "Camera.h"
#include "DescriptorHeap.h"
#include "ImguiWrapper.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ConstBuffer.h"
#include "Object3D.h"
#include "OutlinedObject.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "SRVManager.h"
#include "StructuredBuffer.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"

class MyGame
{
public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();

private:
	// 基盤機能
	Window* window = nullptr;
	DirectXBase* dxBase = nullptr;
	SRVManager* srvManager = nullptr;
	// 汎用機能
	Input* input = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ParticleManager* particleManager = nullptr;
	SoundManager* soundManager = nullptr;

	// その他
};

