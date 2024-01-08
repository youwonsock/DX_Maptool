#include "pch.h"
#include "StructuredBuffer.h"

void StructuredBuffer::CreateInput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetInputByteWidth();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = inputStride;
	desc.Usage = D3D11_USAGE_DYNAMIC;
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
		hr = Global::g_device->CreateBuffer(&desc, nullptr, input.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}
}

void StructuredBuffer::CreateSRV()
{
	D3D11_BUFFER_DESC desc;
	input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.NumElements = inputCount;

	HRESULT hr = Global::g_device->CreateShaderResourceView(input.Get(), &srvDesc, inputSRV.ReleaseAndGetAddressOf() );

	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void StructuredBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetOutputByteWidth();
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = outputStride;

	HRESULT hr = Global::g_device->CreateBuffer(&desc, nullptr, output.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void StructuredBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = outputCount;

	HRESULT hr = Global::g_device->CreateUnorderedAccessView(output.Get(), &uavDesc, outputUAV.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void StructuredBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	output->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = Global::g_device->CreateBuffer(&desc, NULL, result.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		Utils::ShowErrorMessage(hr);
}

void StructuredBuffer::CreateBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
{
	this->inputData = inputData;
	this->inputStride = inputStride;
	this->inputCount = inputCount;
	this->outputStride = outputStride;
	this->outputCount = outputCount;

	if (outputStride == 0 || outputCount == 0)
	{
		outputStride = inputStride;
		outputCount = inputCount;
	}

	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void StructuredBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	Global::g_immediateContext->Map(input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, GetInputByteWidth());
	}
	Global::g_immediateContext->Unmap(input.Get(), 0);
}

void StructuredBuffer::CopuFromOutput(void* data)
{
	Global::g_immediateContext->CopyResource(result.Get(), output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	Global::g_immediateContext->Map(result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, GetInputByteWidth());
	}
	Global::g_immediateContext->Unmap(result.Get(), 0);
}

StructuredBuffer::StructuredBuffer()
{
}

StructuredBuffer::~StructuredBuffer()
{
}
