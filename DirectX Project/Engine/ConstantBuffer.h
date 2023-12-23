#pragma once

template<class T>
class ConstantBuffer
{
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> immediateContext;
	ComPtr<ID3D11Buffer> constantBuffer;

public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> immediateContext)
	{
		this->device = device;
		this->immediateContext = immediateContext;
	}
	~ConstantBuffer()
	{
		constantBuffer.Reset();
	}

public:
	ComPtr<ID3D11Buffer> GetConstantBuffer() { return constantBuffer; }

	void Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(T);

		HRESULT hr = device->CreateBuffer(&desc, NULL, constantBuffer.ReleaseAndGetAddressOf());

		if(FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}

	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		immediateContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &data, sizeof(T));
		immediateContext->Unmap(constantBuffer.Get(), 0);
	}
};

