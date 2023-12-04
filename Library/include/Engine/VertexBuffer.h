#pragma once

class VertexBuffer
{
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Buffer> vertexBuffer;

	UINT stride = 0;
	UINT offset = 0;
	UINT vertexCount = 0;

public:
	VertexBuffer();

	ComPtr<ID3D11Buffer> GetVertexBuffer() { return vertexBuffer; }
	UINT GetStride() { return stride; }
	UINT GetOffset() { return offset; }
	UINT GetVertexCount() { return vertexCount; }

public:
	template <typename T>
	void CreateVertexBuffer(const std::vector<T>& vertexData)
	{
		stride = sizeof(T);
		vertexCount = vertexData.size();

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = stride * vertexCount;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResourceData;
		ZeroMemory(&subResourceData, sizeof(subResourceData));
		subResourceData.pSysMem = vertexData.data();

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subResourceData, vertexBuffer.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			ShowErrorMessage(hr);
	}
};

