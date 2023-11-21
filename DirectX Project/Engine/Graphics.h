#pragma once

class Graphics
{
private:
	ComPtr<ID3D11Device>			pDevice;				
	ComPtr<ID3D11DeviceContext>     pImmediateContext;		
	ComPtr<IDXGISwapChain1>		    pSwapChain;				
	ComPtr<ID3D11RenderTargetView>  pRenderTargetView;		
	D3D11_VIEWPORT					pViewPort;				

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
	void CreateDevice();						  
	void CreateSwapChain(int wWidth, int wHeight);
	void CreateViewPort(int topLeftX, int topLeftY, int width, int height, int minDepth, int maxDepth);
	void CreateRenderTargetView();

public:
	void Init();
	void PreRender();
	void PostRender();
	void Release();
};

