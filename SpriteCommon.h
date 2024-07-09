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

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_;
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	D3D12_BLEND_DESC blendDesc_; // kBlendModeNormal
	D3D12_RASTERIZER_DESC rasterizerDesc_;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;

	D3D12_BLEND_DESC blendDescNone_; // kBlendModeNone
	D3D12_BLEND_DESC blendDescAdd_; // kBlendModeAdd
	D3D12_BLEND_DESC blendDescSubtract_; // kBlendModeSubtract
	D3D12_BLEND_DESC blendDescMultiply_; // kBlendModeMultiply
	D3D12_BLEND_DESC blendDescScreen_; // kBlendModeScreen
	// BlendMode変更用のPSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBlendModeNone_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBlendModeAdd_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBlendModeSubtract_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBlendModeMultiply_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBlendModeScreen_;
};

