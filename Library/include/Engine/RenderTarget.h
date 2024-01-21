#pragma once

#include "Viewport.h"

class RenderTarget
{
public:
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11ShaderResourceView> depthStencilviewSRV;

	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	Viewport viewport;

	Matrix projectionMat;

	// store old render target
	ComPtr<ID3D11RenderTargetView> oldRenderTarget;
	ComPtr<ID3D11DepthStencilView> oldDepthStencilviewSRV;
	UINT numViewports;
	D3D11_VIEWPORT oldViewport[15];

private:
	void Set(float topLeftX = 0, float topLeftY = 0, float width = 100, float height = 100, float mindepth = 0, float maxdepth = 1);

public:
	HRESULT Create(float width, float height);
	HRESULT UpdateDepthStencilView(UINT width, UINT height);
	
	void Begin();
	void End();

//	std::shared_ptr<Texture> GetTexture() { return texture; }

	RenderTarget();
	~RenderTarget();
};

