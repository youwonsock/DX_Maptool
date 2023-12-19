#include "pch.h"
#include "Graphics.h"

#include "ExLib.h"

void Graphics::CreateViewPort(int topLeftX, int topLeftY, int width, int height, int minDepth, int maxDepth)
{
    pViewPort.TopLeftX = topLeftX;
    pViewPort.TopLeftY = topLeftY;
    pViewPort.Width = width;
    pViewPort.Height = height;
    pViewPort.MinDepth = minDepth;
    pViewPort.MaxDepth = maxDepth;
}

void Graphics::CreateRenderTargetView()
{
    // get back buffer
    ComPtr<ID3D11Texture2D> pBackBuffer;
    HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.ReleaseAndGetAddressOf()); 

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

    // set render target view
    hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.ReleaseAndGetAddressOf()); 
    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);
}

void Graphics::CreateDepthStencilView()
{
    // set depth stencil description
	D3D11_TEXTURE2D_DESC depthStencilDesc; 
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC)); 
	depthStencilDesc.Width = Global::g_windowWidth; 
	depthStencilDesc.Height = Global::g_windowHeight; 
	depthStencilDesc.MipLevels = 1; 
	depthStencilDesc.ArraySize = 1; 
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	depthStencilDesc.SampleDesc.Count = 1; 
	depthStencilDesc.SampleDesc.Quality = 0; 
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; 
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; 
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags = 0; 

	// create depth stencil buffer
	HRESULT hr = pDevice->CreateTexture2D(&depthStencilDesc, NULL, pDepthStencilBuffer.ReleaseAndGetAddressOf()); 
	if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

	// set depth stencil view description
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; 
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC)); 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; 
	depthStencilViewDesc.Texture2D.MipSlice = 0; 

	// create depth stencil view
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), &depthStencilViewDesc, pDepthStencilView.ReleaseAndGetAddressOf()); 
	if (FAILED(hr))
        Utils::ShowErrorMessage(hr);
}

void Graphics::CreateDevice()
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0; // set feature level (directx version) : we use directx 11
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;           // for dx write

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
    
    //flags |= D3D11_CREATE_DEVICE_DEBUGGABLE; ???
#endif // !_DEBUG

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        &featureLevel, 1,
        D3D11_SDK_VERSION,
        pDevice.ReleaseAndGetAddressOf(),
        nullptr,
        pImmediateContext.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

    Global::g_device = pDevice;
    Global::g_immediateContext = pImmediateContext;
}

void Graphics::CreateSwapChain(int wWidth, int wHeight)
{
    // create IDXGIFactory2 for use CreateSwapChainHwnd
    IDXGIFactory2* pDxgiFactory = nullptr;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (LPVOID*)&pDxgiFactory);

    if (FAILED(hr)) // failed to create IDXGIFactory2
        Utils::ShowErrorMessage(hr);

    // set swap chain(back buffer) description
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;  
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1)); 
    swapChainDesc.Width = wWidth; 
    swapChainDesc.Height = wHeight; 
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    swapChainDesc.SampleDesc.Count = 1; 
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    swapChainDesc.BufferCount = 1; 

    // create swap chain
    hr = pDxgiFactory->CreateSwapChainForHwnd(
        pDevice.Get(),
        Global::g_hWnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        pSwapChain.ReleaseAndGetAddressOf()
    );
    pDxgiFactory->Release();

    if (FAILED(hr)) // failed to create swap chain
        Utils::ShowErrorMessage(hr);
}

void Graphics::Init()
{
    CreateDevice();
    CreateSwapChain( Global::g_windowWidth, Global::g_windowHeight);

    CreateRenderTargetView();
    CreateDepthStencilView();

    CreateViewPort(0, 0, Global::g_windowWidth, Global::g_windowHeight, 0.0f, 1.0f);
}

void Graphics::PreRender()
{
    pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
    pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), (float*)&clearColor);
    pImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    pImmediateContext->RSSetViewports(1, &pViewPort);
                
}

void Graphics::PostRender()
{
    pSwapChain->Present(0, 0); 
}

void Graphics::Release()
{

}
