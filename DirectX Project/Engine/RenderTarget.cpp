#include "pch.h"
#include "RenderTarget.h"

HRESULT RenderTarget::Create(float width, float height)
{
	HRESULT hr;

	Set(0, 0, width, height);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1; // multisampling setting
	textureDesc.SampleDesc.Quality = 0; // vendor-specific flag
	textureDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_DEFAULT
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	textureDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_WRITE
	textureDesc.MiscFlags = 0;

	hr = Global::g_device->CreateTexture2D(&textureDesc, nullptr, texture.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = Global::g_device->CreateShaderResourceView(texture.Get(), nullptr, textureSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = Global::g_device->CreateRenderTargetView(texture.Get(), nullptr, renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = UpdateDepthStencilView(width, height);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT RenderTarget::UpdateDepthStencilView(UINT width, UINT height)
{
	HRESULT hr;

	viewport.SetWidth(width);
	viewport.SetHeight(height);

	ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1; // multisampling setting
	depthStencilDesc.SampleDesc.Quality = 0; // vendor-specific flag
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_DEFAULT
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
	depthStencilDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_WRITE
	depthStencilDesc.MiscFlags = 0;

	hr = Global::g_device->CreateTexture2D(&depthStencilDesc, nullptr, pDSTexture.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = Global::g_device->CreateDepthStencilView(pDSTexture.Get(), &depthStencilViewDesc, depthStencilView.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	hr = Global::g_device->CreateShaderResourceView(pDSTexture.Get(), &SRVDesc, depthStencilviewSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void RenderTarget::Set(float topLeftX, float topLeftY, float width, float height, float mindepth, float maxdepth)
{
	viewport.SetViewport(width, height, topLeftX, topLeftY, mindepth, maxdepth);

	projectionMat = Matrix::CreatePerspectiveOffCenter(topLeftX, topLeftX + width, topLeftY, topLeftY + height, 0.1f, 1000.0f);
}

void RenderTarget::Begin()
{
	// store old render target
	numViewports = 1;
	Global::g_immediateContext->RSGetViewports(&numViewports, oldViewport);
	Global::g_immediateContext->OMGetRenderTargets(1, oldRenderTarget.GetAddressOf(), oldDepthStencilviewSRV.GetAddressOf());

	// set render target
	ComPtr<ID3D11RenderTargetView> pRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	Global::g_immediateContext->OMSetRenderTargets(1, pRTV.GetAddressOf(), pDSV.Get());

	ComPtr<ID3D11ShaderResourceView> ppSRVNULL[2] = { NULL, NULL };
	Global::g_immediateContext->PSSetShaderResources(3, 2, ppSRVNULL->GetAddressOf());


	float clearColor[4] = { 1, 1, 1, 1 };
	Global::g_immediateContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
	Global::g_immediateContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Global::g_immediateContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	viewport.RSSetViewport();
}

void RenderTarget::End()
{
	ComPtr<ID3D11RenderTargetView> pRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	Global::g_immediateContext->OMSetRenderTargets(1, pRTV.GetAddressOf(), pDSV.Get());

	ComPtr<ID3D11ShaderResourceView> ppSRVNULL[2] = { NULL, NULL };
	Global::g_immediateContext->PSSetShaderResources(3, 2, ppSRVNULL->GetAddressOf());

	Global::g_immediateContext->RSSetViewports(numViewports, oldViewport);
	Global::g_immediateContext->OMSetRenderTargets(1, oldRenderTarget.GetAddressOf(), oldDepthStencilviewSRV.Get());

	oldRenderTarget.Reset();
	oldDepthStencilviewSRV.Reset();
}

RenderTarget::RenderTarget()
{
	renderTargetView = nullptr;
	depthStencilView = nullptr;
	depthStencilviewSRV = nullptr;
	texture = nullptr;
	textureSRV = nullptr;
}

RenderTarget::~RenderTarget()
{
}
