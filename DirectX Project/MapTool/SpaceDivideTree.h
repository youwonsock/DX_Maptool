#pragma once

class SectionNode;
class Terrain;
class RenderMgr;
class DebugDrawer;

class SpaceDivideTree
{
public:
	int maxDepth = 4;
	
	std::weak_ptr<Terrain> terrain;
	std::shared_ptr<SectionNode> root;

	// temp : for debug
	std::shared_ptr<DebugDrawer> debugDraw;

	std::vector<UINT> leafNodeIndexList;
	std::shared_ptr<IndexBuffer> leafNodeIndexBuffer;

	std::map<int, std::shared_ptr<SectionNode>> leafNodeMap;
	std::vector<int> drawNodeIndexList;
private:
	void FindDrawNode();

	// build tree
	void CreateBoundingBox(std::shared_ptr<SectionNode> pNode);
	void BuildTree(std::shared_ptr<SectionNode> pNode);
	void UpdateVertexList(std::shared_ptr<SectionNode> pNode);
	bool SubDivide(std::shared_ptr<SectionNode> pNode);
	void SetNeighborNode();

	Vector2 GetHeightFromNode(std::shared_ptr<SectionNode> pNode);
	UINT CheckSize(UINT dwSize);

	std::shared_ptr<SectionNode> CreateNode(std::shared_ptr<SectionNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB);

	// create index buffer
	void CreateIndexBuffer(UINT rowNum, UINT colNum);

public:
	void Init();
	void Update();
	void Render();

	void UpdateVertex(std::vector<SHORT> updateNodeIdxList);

	SpaceDivideTree(std::shared_ptr<Terrain> owner);
	~SpaceDivideTree();
};

