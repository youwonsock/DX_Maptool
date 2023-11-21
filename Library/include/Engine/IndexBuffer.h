#pragma once

class IndexBuffer
{
private:
	ComPtr<ID3D11Buffer> indexBuffer;

	UINT stride;
	UINT offset;
	UINT indexCount;
public:
	ComPtr<ID3D11Buffer> GetIndexBuffer() { return indexBuffer; }
	UINT GetStride() { return stride; }
	UINT GetOffset() { return offset; }
	UINT GetIndexCount() { return indexCount; }

public:
	void CreateIndexBuffer(const std::vector<UINT>& indices);
};

