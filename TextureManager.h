#pragma once
#include <d3d12.h>
#include "externals/DirectXTex/DirectXTex.h"
#include "StringUtil.h"
#include "Logger.h"
#include "DescriptorHeap.h"

class TextureManager final
{
public:
	static void Initialize(ID3D12Device* device);

	static int Load(const std::string& filePath, ID3D12Device* device);

	static TextureManager& GetInstance();

	static void SetDescriptorTable(UINT rootParamIndex, ID3D12GraphicsCommandList* commandList, uint32_t textureHandle);

	DescriptorHeap srvHeap_;
private:
	// TextureデータをCPUで読む
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);
	// DirectX12のTextureResourceを作る
	static ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	// TextureResourceにデータを転送する
	static void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	uint32_t index_ = 1;
};

