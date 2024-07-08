#pragma once
#include "DirectXBase.h"

class SpriteCommon
{
public:
	// 初期化
	void Initialize(DirectXBase* dxBase);

	// 共通描画設定
	void PreDraw();

	// DxBaseのgetter
	DirectXBase* GetDxBase() const { return dxBase_; }

private:
	DirectXBase* dxBase_;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();
};

