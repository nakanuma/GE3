#pragma once
#include "DirectXBase.h"
#include "SRVManager.h"

class ParticleManager
{
public:
	ParticleManager* GetInstance();

	void Initialize(DirectXBase* dxBase, SRVManager* srvManager);
private:
	DirectXBase* dxBase = nullptr;
	SRVManager* srvManager = nullptr;
};

