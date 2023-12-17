#include "pch.h"
#include "InstancingBuffer.h"

#include "VertexBuffer.h"

InstancingBuffer::InstancingBuffer()
{
	CreateBuffer(MAX_MESH_INSTANCE);
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::CreateBuffer(UINT maxCount)
{
	this->maxCount = maxCount;
	instanceBuffer = std::make_shared<VertexBuffer>();

	std::vector<InstancingData> temp(maxCount);
	instanceBuffer->CreateVertexBuffer(temp, 1, true);
}

void InstancingBuffer::ClearData()
{
	instanceDatas.clear();
}

void InstancingBuffer::AddData(InstancingData& data)
{
	instanceDatas.push_back(data);
}

void InstancingBuffer::PushData()
{
	const UINT dataCount = GetDataCount();

	if(dataCount > maxCount)
		CreateBuffer(dataCount);

	D3D11_MAPPED_SUBRESOURCE subResource;

	Global::g_immediateContext->Map(instanceBuffer->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, instanceDatas.data(), sizeof(InstancingData) * dataCount);
	}
	Global::g_immediateContext->Unmap(instanceBuffer->GetVertexBuffer().Get(), 0);

	instanceBuffer->PushData();
}
