#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <cstdint>
#include <dxcapi.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

// MyClass
#include "MyWindow.h"
#include "DescriptorHeap.h"

class DirectXBase
{
public:
	// シングルトンインスタンスの取得
	static DirectXBase* GetInstance();

	// 初期化
	void Initialize();

	// DXGIデバイス初期化
	void InitializeDXGIDevice(bool enableDebugLayer = true);
	// コマンド関連初期化
	void InitializeCommand();
	// スワップチェーンの生成
	void CreateSwapChain();
	// レンダーターゲット生成
	void CreateFinalRenderTargets();
	// フェンス生成
	void CreateFence();
	// DXC初期化
	void InitializeDXC();
	// RootSignature生成
	void CreateRootSignature();
	// InputLayoutの設定
	void SetInputLayout();
	// BlendStateの設定
	D3D12_BLEND_DESC SetBlendState();
	// RasterizerStateの設定
	D3D12_RASTERIZER_DESC SetRasterizerState();
	// Shaderのコンパイル
	void ShaderCompile();
	// PSO生成
	void CreatePipelineStateObject();
	// Viewportの設定
	void SetViewport();
	// Scissorの設定
	void SetScissor();

	// フレーム開始処理
	void BeginFrame();
	// フレーム終了処理
	void EndFrame();

	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

	// アクセッサ
	Microsoft::WRL::ComPtr <ID3D12Device> GetDevice();
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> GetCommandList();
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc();
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
	DescriptorHeap rtvDescriptorHeap_;
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources_[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_RESOURCE_BARRIER barrier_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_;
	D3D12_BLEND_DESC blendDesc_;
	D3D12_RASTERIZER_DESC rasterizerDesc_;
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;
};

