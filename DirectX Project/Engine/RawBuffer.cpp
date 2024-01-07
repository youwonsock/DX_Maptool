#include "pch.h"
#include "RawBuffer.h"

void RawBuffer::CreateInput()
{
	if (inputByte == 0)
		return;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = inputByte;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; // RAW_BUFFER
	desc.Usage = D3D11_USAGE_DYNAMIC;							 // CPU-WRITE, GPU-READ
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	HRESULT hr;
	if (inputData != nullptr)
	{
		hr = Global::g_device->CreateBuffer(&desc, &subResource, input.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}
	else
	{
		hr = Global::g_device->CreateBuffer(&desc, nullptr, input.ReleaseAndGetAddressOf()); // crash 방지용 예외처리
		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}
}

void RawBuffer::CreateSRV()
{
	if (inputByte == 0)
		return;
	D3D11_BUFFER_DESC desc;
	input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;				// 타입은 쉐이더에서 자동으로 판단
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;	// SRV_FLAG_RAW
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;		// 전체 데이터 개수

	HRESULT hr = Global::g_device->CreateShaderResourceView(input.Get(), &srvDesc, inputSRV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void RawBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = outputByte;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	HRESULT hr = Global::g_device->CreateBuffer(&desc, NULL, output.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void RawBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;				  // 타입은 쉐이더에서 자동으로 판단
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;		  // SRV_FLAG_RAW
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;		  // 전체 데이터 개수

	HRESULT hr = Global::g_device->CreateUnorderedAccessView(output.Get(), &uavDesc, outputUAV.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void RawBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	output->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = D3D11_USAGE_DEFAULT;			// uav를 연결하기 위해선 default로 설정해야함
	desc.MiscFlags = 0;

	HRESULT hr = Global::g_device->CreateBuffer(&desc, NULL, result.ReleaseAndGetAddressOf());
	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void RawBuffer::CreateBuffer()
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void RawBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	Global::g_immediateContext->Map(input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, inputByte);
	}
	Global::g_immediateContext->Unmap(input.Get(), 0);
}

void RawBuffer::CopuFromOutput(void* data)
{
	Global::g_immediateContext->CopyResource(result.Get(), output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	Global::g_immediateContext->Map(result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, outputByte);
	}
	Global::g_immediateContext->Unmap(result.Get(), 0);
}

RawBuffer::RawBuffer(void* inputData, UINT inputByte, UINT outputByte) 	: inputData(inputData), inputByte(inputByte), outputByte(outputByte)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{
}
