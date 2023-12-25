#pragma once

class SectionNode;

struct LODData
{
	DWORD level = 0;
	std::vector<std::vector<UINT>>	indicesList;
	std::vector<std::shared_ptr<IndexBuffer>>	indexBufferList;
};

class StaticLOD
{
public:
	DWORD cellNum = 0;
	DWORD patchLodCount = 0;

	std::vector<std::shared_ptr<LODData>> lodDataList;
private:
	void GetLodSubIndex(std::shared_ptr<SectionNode> pNode);
	void GetLodType(std::shared_ptr<SectionNode> pNode);
	float GetExpansionRatio(Vector3 center);
	void ComputeStaticLodIndex();

	//create index data, index buffer
	void CreateIndexBuffer(std::shared_ptr<LODData>& pNode);
	DWORD GetIndexCount(DWORD data, DWORD cellNum);
public:
	void SetLod(DWORD width, DWORD numDepth);
	std::shared_ptr<IndexBuffer>& GetLodIndexBuffer(std::shared_ptr<SectionNode>& pNode);

	void Release();
};

