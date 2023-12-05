#pragma once

#include "Types.h"

class Graphics
{
private:
	ComPtr<ID3D11Device>			pDevice;				
	ComPtr<ID3D11DeviceContext>     pImmediateContext;		
	ComPtr<IDXGISwapChain1>		    pSwapChain;				
	ComPtr<ID3D11RenderTargetView>  pRenderTargetView;		
	ComPtr<ID3D11Texture2D>			pDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView>  pDepthStencilView;
	D3D11_VIEWPORT					pViewPort;				

	Color clearColor = { 0.5f, 0.5f, 0.5f, 1.0f };
private:
	void CreateDevice();						  
	void CreateSwapChain(int wWidth, int wHeight);
	void CreateViewPort(int topLeftX, int topLeftY, int width, int height, int minDepth, int maxDepth);
	void CreateRenderTargetView();
	void CreateDepthStencilView();

public:
	void Init();
	void PreRender();
	void PostRender();
	void Release();
};

