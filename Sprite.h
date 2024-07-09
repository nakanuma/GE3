#pragma once
#include "MyMath.h"
#include "DirectXBase.h"

class SpriteCommon; // 前方宣言

class Sprite
{
public:
	struct VertexData {
		Float4 position;
		Float2 texcoord;
		Float3 normal;
	};

	struct Material {
		Float4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix uvTransform;
	};

	struct TransformationMatrix {
		Matrix WVP;
		Matrix World;
	};

	// 初期化
	void Initialize(SpriteCommon* spriteCommon);
	// 更新
	void Update();
	// 描画
	void Draw(uint32_t textureHandle);

	///
	///	アクセッサ
	/// 

	// 座標
	const Float2 GetPosition() const { return position_; }
	void SetPosition(const Float2& position) { this->position_ = position; }
	// 回転
	float GetRotation() const { return rotation; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	// 色
	const Float4& GetColor() const { return materialData_->color; }
	void SetColor(const Float4& color) { materialData_->color = color; }
	// サイズ
	const Float2& GetSize() const { return size; }
	void SetSize(const Float2& size) { this->size = size; }

private:
	SpriteCommon* spriteCommon = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	Material* materialData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	// Transform
	Transform transform_;

	// 座標
	Float2 position_ = { 0.0f, 0.0f };
	// 回転
	float rotation = 0.0f;
	// サイズ
	Float2 size = { 640.0f, 360.0f };
};

