#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXBase
{
public:
	// シングルトンインスタンスの取得
	static DirectXBase* GetInstance();

	// DXGIデバイス初期化
	void InitializeDXGIDevice(bool enableDebugLayer = true);

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};

