#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance()
{
    static ParticleManager instance;
    return &instance;
}

void ParticleManager::Initialize(DirectXBase* dxBase, SRVManager* srvManager)
{
    // 引数で受け取ったポインタをメンバ変数に記録
    this->dxBase = dxBase;
    this->srvManager = srvManager;


}
