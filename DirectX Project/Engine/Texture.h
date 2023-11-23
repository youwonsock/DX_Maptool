#pragma once

#include "Types.h"
#include "ResourceBase.h"

class Texture : public ResourceBase
{
private:
	using Base = ResourceBase;
	
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Vector2 size;

public:
	Texture();
	virtual ~Texture();

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourceView; }

	virtual void Load(const std::wstring& path) override;
	Vector2& GetSize() { return size; };
};

