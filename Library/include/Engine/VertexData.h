#pragma once

struct VertexTextureData
{
	Vector3 position = {0,0,0};
	Vector2 UV = {0,0};

	VertexTextureData() = default;
	VertexTextureData(const Vector3& pos, const Vector2& uv)
		: position(pos), UV(uv)
	{}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vector3 position = {0,0,0};
	Color color = {0,0,0,0};

	VertexColorData() = default;
	VertexColorData(const Vector3& pos, const Color& col)
		: position(pos), color(col)
	{}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

