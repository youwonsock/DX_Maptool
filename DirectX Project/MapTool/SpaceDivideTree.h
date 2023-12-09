#pragma once

class SectionNode;
class Terrain;
class RenderMgr;

class SpaceDivideTree
{
public:
	int maxDepth = 2;
	int minDivideSize = 1;
	float minSplitDistance = 2.0f;
	
	std::weak_ptr<Terrain> terrain;
	std::shared_ptr<SectionNode> root;

	// temp render mgr
	std::shared_ptr<RenderMgr> renderMgr;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<IndexBuffer> leafNodeIndexBuffer;

	std::vector<std::shared_ptr<SectionNode>> leafNodeList;
	std::vector<int> drawNodeIndexList;
private:
	void FindDrawNode();

	// build tree
	void CreateBoundingBox(std::shared_ptr<SectionNode> pNode);
	void BuildTree(std::shared_ptr<SectionNode> pNode);
	void UpdateVertexList(std::shared_ptr<SectionNode> pNode);
	bool SubDivide(std::shared_ptr<SectionNode> pNode);

	Vector2 GetHeightFromNode(std::shared_ptr<SectionNode> pNode);
	UINT CheckSize(UINT dwSize);

	std::shared_ptr<SectionNode> CreateNode(std::shared_ptr<SectionNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB);

	// create index buffer
	void CreateIndexBuffer(UINT rowNum, UINT colNum);

public:
	void Init();
	void Update();
	void Render();

	SpaceDivideTree(std::shared_ptr<Terrain> owner);
	~SpaceDivideTree();
};
