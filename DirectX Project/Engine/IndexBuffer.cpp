#include "pch.h"
#include "IndexBuffer.h"

void IndexBuffer::CreateIndexBuffer(const std::vector<UINT>& indices)
{
	stride = sizeof(UINT);
	indexCount = indices.size();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	HRESULT hr = Global::g_device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create index buffer", L"Error", MB_OK);
	}
}
