#pragma once

class SectionNode;
class Terrain;
class RenderMgr;
class Frustum;
class DebugDrawer;

class SpaceDivideTree
{
public:
	int maxDepth = 4;
	
	std::weak_ptr<Terrain> terrain;
	std::shared_ptr<SectionNode> root;

	// temp render mgr
	std::shared_ptr<RenderMgr> renderMgr;
	// temp Frustum
	std::shared_ptr<Frustum> frustum;
	// temp : for debug
	std::shared_ptr<DebugDrawer> debugDraw;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
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

	// temp : for picking
	void UpdateVertex();

	SpaceDivideTree(std::shared_ptr<Terrain> owner);
	~SpaceDivideTree();
};

