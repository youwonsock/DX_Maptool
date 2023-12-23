#pragma once

class VertexBuffer
{
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Buffer> vertexBuffer;

	UINT stride = 0;
	UINT offset = 0;
	UINT vertexCount = 0;

	UINT slot = 0;
	bool cpuWrite = false;
	bool gpuWrite = false;
public:
	VertexBuffer();
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetVertexBuffer() { return vertexBuffer; }
	UINT GetStride() { return stride; }
	UINT GetOffset() { return offset; }
	UINT GetVertexCount() { return vertexCount; }
	UINT GetSlot() { return slot; }

public:
	template <typename T>
	void CreateVertexBuffer(const std::vector<T>& vertexData, UINT slot = 0, bool cpuWrite = false, bool gpuWrite = false)
	{
		stride = sizeof(T);
		vertexCount = vertexData.size();

		this->slot = slot;
		this->cpuWrite = cpuWrite;
		this->gpuWrite = gpuWrite;
		
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = stride * vertexCount;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		if(!cpuWrite && !gpuWrite)
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;	// cpu, gpu read
		else if (cpuWrite && !gpuWrite)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;		// cpu write, gpu read
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else if (!cpuWrite && gpuWrite)
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;		// cpu read, gpu write
		else if (cpuWrite && gpuWrite)
		{
			bufferDesc.Usage = D3D11_USAGE_STAGING;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA subResourceData;
		ZeroMemory(&subResourceData, sizeof(subResourceData));
		subResourceData.pSysMem = vertexData.data();

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subResourceData, vertexBuffer.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}

	template <typename T>
	void UpdateVertexBuffer(const std::vector<T>& vertexData)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		HRESULT hr = Global::g_immediateContext->Map(vertexBuffer.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&subResource);

		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);

		memcpy(subResource.pData, vertexData.data(), sizeof(T) * vertexData.size());

		Global::g_immediateContext->Unmap(vertexBuffer.Get(), 0);
	}

	void PushData();
};

