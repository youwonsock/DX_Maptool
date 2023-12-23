#pragma once

class GridMesh;
class SectionNode;

struct SectionNodeDesc
{
	std::shared_ptr<SectionNode> pParent;
	
	UINT LT;
	UINT RT;
	UINT LB;
	UINT RB;
	
	UINT colNum;
};

class SectionNode : public std::enable_shared_from_this<SectionNode>
{
private:

public:
	bool isLeafNode = false;
	int depth = 0;
	UINT nodeIndex = 0;
	UINT lodLevel = 0;
	UINT lodType = 0;

	Cube boundingBox;
	Vector2 element;

	std::weak_ptr<SectionNode> parentNode;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::vector<PNCTVertex> vertices;
	std::vector<UINT> cornerIndexList;
	std::vector<std::shared_ptr<SectionNode>> childNodeList;
	std::vector<std::shared_ptr<SectionNode>> neighborNodeList;

private:
	Vector2 GetHeight();

public:
	void Render();

	void SetVertexBuffer();
	void UpdateVertexBuffer();
	void SetBoundingBox();

	SectionNode();
	SectionNode(SectionNodeDesc& desc);
};

