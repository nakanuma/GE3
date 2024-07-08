#include "SpriteCommon.h"

void SpriteCommon::Initialize(DirectXBase* dxBase)
{
	// 引数で受け取ってメンバ変数に記録する
	dxBase_ = dxBase;

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();
}

void SpriteCommon::PreDraw()
{
	// ルートシグネチャをセット

	// グラフィックスパイプラインステートをセット

	// プリミティブトポロジーをセット
}

void SpriteCommon::CreateRootSignature()
{

}

void SpriteCommon::CreateGraphicsPipeline()
{
	// ルートシグネチャの作成
	CreateRootSignature();
}
