#pragma once

#include "Types.h"
#include "ResourceBase.h"

class Texture : public ResourceBase
{
private:
	using Base = ResourceBase;
	
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Vector2 size;

private:

public:
	Texture();
	virtual ~Texture();

	virtual bool Load(const std::wstring& path) override;
	
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourceView; }
	ComPtr<ID3D11Texture2D> GetTexture2D();
	Vector2 GetSize() { return size; }
	const std::shared_ptr<DirectX::ScratchImage> GetInfo();

	void SetShaderResourceView(ComPtr<ID3D11ShaderResourceView> srv) { shaderResourceView = srv; }

	void CreateTexture(int width, int height);
	void UpdateTexture(const std::vector<BYTE>& colors);
	void SaveTexture(const std::wstring& path);

	void GetTextureRGBAData(std::vector<BYTE>& colors);

	// cubemap 
	void LoadCubemapTexture(const std::wstring& path);
	void CreateCubemapTexture(float width, float hight);
};

