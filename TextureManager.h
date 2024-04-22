#pragma once
#include <d3d12.h>
#include "externals/DirectXTex/DirectXTex.h"
#include "StringUtil.h"
#include "DescriptorHeap.h"

class TextureManager final
{
public:
	
public:
	// TextureデータをCPUで読む
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);
	// DirectX12のTextureResourceを作る
	static ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	// TextureResourceにデータを転送する
	static void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
};

