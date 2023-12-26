#pragma once

struct VertexData
{
	Vector3 position = { 0, 0, 0 };

	VertexData() = default;
	VertexData(const Vector3& pos) : position(pos) {}
};

struct VertexTextureNormalData
{
	Vector3 position = { 0, 0, 0 };
	Vector3 normal = { 0, 0, 0 };
	Vector2 uv = { 0, 0 };

	VertexTextureNormalData() = default;
	VertexTextureNormalData(const Vector3& pos, const Vector2& uv, const Vector3& normal) : position(pos), uv(uv), normal(normal) {}
};

struct VertexTextureNormalTangentData
{
	Vector3 position = { 0, 0, 0 };
	Vector3 normal = { 0, 0, 0 };
	Vector2 uv = { 0, 0 };
	Vector3 tangent = { 0, 0, 0 };

	VertexTextureNormalTangentData() = default;
	VertexTextureNormalTangentData(const Vector3& pos, const Vector2& uv, const Vector3& normal, const Vector3& tangent) : position(pos), uv(uv), normal(normal), tangent(tangent) {}
};


struct PNCTVertex
{
	Vector3 position = { 0, 0, 0 };
	Vector3 normal = { 0, 0, 0 };
	Vector4 color = { 0, 0, 0, 0 };
	Vector2 uv = { 0, 0 };

	PNCTVertex() = default;
	PNCTVertex(const Vector3& pos, const Vector2& uv, const Vector4& color, const Vector3& normal) : position(pos), uv(uv), normal(normal), color(color){}
};

struct VertexIWT
{
	Vector4 blendIndices = { 0, 0, 0, 0 };
	Vector4 blendWeights = { 0, 0, 0, 0 };
	Vector3 tangent = { 0, 0, 0 };

	VertexIWT() = default;
	VertexIWT(const Vector4& blendIndices, const Vector4& blendWeights, const Vector3& tangent) : blendIndices(blendIndices), blendWeights(blendWeights), tangent(tangent) {}
};

struct VertexTextureNormalTangentBlendData
{
	Vector3 position = { 0, 0, 0 };
	Vector3 normal = { 0, 0, 0 };
	Vector2 uv = { 0, 0 };
	Vector3 tangent = { 0, 0, 0 };
	Vector4 blendIndices = { 0, 0, 0, 0 };
	Vector4 blendWeights = { 0, 0, 0, 0 };

	VertexTextureNormalTangentBlendData() = default;
	VertexTextureNormalTangentBlendData(const Vector3& pos, const Vector2& uv, const Vector3& normal, const Vector3& tangent, const Vector4 blendIndices, const Vector4 blendWeights)
		: position(pos), uv(uv), normal(normal), tangent(tangent), blendIndices(blendIndices), blendWeights(blendWeights) {}
};

struct VertexTextureData
{
	Vector3 position = {0,0,0};
	Vector2 UV = {0,0};

	VertexTextureData() = default;
	VertexTextureData(const Vector3& pos, const Vector2& uv) : position(pos), UV(uv) {}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vector3 position = {0,0,0};
	Color color = {0,0,0,0};

	VertexColorData() = default;
	VertexColorData(const Vector3& pos, const Color& col) : position(pos), color(col) {}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

using ModelVertexType = VertexTextureNormalTangentBlendData;