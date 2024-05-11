#pragma once
#include <vector>
#include <string>
#include <d3d12.h>

// MyClass
#include "MyMath.h"
#include "TextureManager.h"

struct VertexData {
	Float4 position;
	Float2 texcoord;
	Float3 normal;
};

struct MaterialData {
	std::string textureFilePath;
	uint32_t textureHandle;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

class ModelManager
{
public:
	// Objファイルの読み込みを行う
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename, ID3D12Device* device);
	// mtlファイルの読み込みを行う
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename, ID3D12Device* device);
};

