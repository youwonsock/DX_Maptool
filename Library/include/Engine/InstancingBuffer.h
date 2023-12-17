#pragma once

class VertexBuffer;

struct InstancingData
{
	Matrix world;
};

#define MAX_MESH_INSTANCE 1000

class InstancingBuffer
{
private:
	UINT instanceID = 0;
	UINT maxCount = 0;
	std::shared_ptr<VertexBuffer> instanceBuffer;
	std::vector<InstancingData> instanceDatas;

public:

private:
	void CreateBuffer(UINT maxCount = MAX_MESH_INSTANCE);

public:
	InstancingBuffer();
	~InstancingBuffer();

	void ClearData();
	void AddData(InstancingData& data);
	void PushData();

	UINT GetDataCount() { return instanceDatas.size(); }
	std::shared_ptr<VertexBuffer> GetInstanceBuffer() { return instanceBuffer; }
};

