#pragma once

class GridMesh;

class SectionNode : public std::enable_shared_from_this<SectionNode>
{
private:

public:
	bool isLeafNode = false;
	int depth = 0;
	int nodeIndex = 0;
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

public:
	void Render();

	void SetVertexBuffer();
	void UpdateVertexBuffer();
};

