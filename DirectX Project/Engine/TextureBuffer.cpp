#include "pch.h"
#include "TextureBuffer.h"

void TextureBuffer::CreateBuffer(ComPtr<ID3D11Texture2D> src)
{
	CreateInput(src);
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void TextureBuffer::CreateInput(ComPtr<ID3D11Texture2D> src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	width = srcDesc.Width;
	height = srcDesc.Height;
	arraySize = srcDesc.ArraySize;
	format = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; // D3D11_BIND_UNORDERED_ACCESS : 여러 스레드에서 임시로 순서가 지정되지 않은 읽기/쓰기 엑세스 허용
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	HRESULT hr = Global::g_device->CreateTexture2D(&desc, nullptr, input.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);

	Global::g_immediateContext->CopyResource(input.Get(), src.Get());
}

void TextureBuffer::CreateSRV()
{
	D3D11_TEXTURE2D_DESC desc;
	input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; 
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2DArray.ArraySize = arraySize;

	HRESULT hr = Global::g_device->CreateShaderResourceView(input.Get(), &srvDesc, srv.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void TextureBuffer::CreateOutput()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;


	HRESULT hr = Global::g_device->CreateTexture2D(&desc, nullptr, output.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void TextureBuffer::CreateUAV()
{
	D3D11_TEXTURE2D_DESC desc;
	output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	//uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = arraySize;

	HRESULT hr = Global::g_device->CreateUnorderedAccessView(output.Get(), &uavDesc, uav.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void TextureBuffer::CreateResult()
{
	D3D11_TEXTURE2D_DESC desc;
	output->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = arraySize;
	srvDesc.Texture2DArray.MipLevels = 1;

	HRESULT hr = Global::g_device->CreateShaderResourceView(output.Get(), &srvDesc, outputSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

TextureBuffer::TextureBuffer()
{
	
}

TextureBuffer::~TextureBuffer()
{
}
