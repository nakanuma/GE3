#pragma once
#include "ParticleEmitter.h"
#include "ParticleManager.h"

class ParticleEmitter
{
public:

private:
	// パーティクルグループコンテナ
	std::unordered_map<std::string, std::unique_ptr<ParticleManager::ParticleGroup>> particleGroups;
};

