#pragma once

class SectionNode;
class Terrain;
class RenderMgr;
class DebugDrawer;
class StaticLOD;
class ObjectManager;

class SpaceDivideTree
{
public:
	int maxDepth = 4;
	
	std::weak_ptr<Terrain> terrain;
	std::shared_ptr<SectionNode> root;

	std::map<int, std::shared_ptr<SectionNode>> leafNodeMap;
	std::vector<int> drawNodeIndexList;

	std::shared_ptr<StaticLOD> staticLOD;
	std::shared_ptr<ObjectManager> objectManager;

	// temp : for debug
	std::shared_ptr<DebugDrawer> debugDraw;
private:
	// update
	void FindDrawNode();
	void UpdateVertexList(std::shared_ptr<SectionNode> pNode);

	// build tree
	void BuildTree(std::shared_ptr<SectionNode> pNode);
	void SetNeighborNode();
	bool SubDivide(std::shared_ptr<SectionNode> pNode);
	UINT CheckSize(UINT dwSize);
	std::shared_ptr<SectionNode> CreateNode(std::shared_ptr<SectionNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB);

public:
	void UpdateVertex(std::vector<SHORT> updateNodeIdxList);
	
	void SpawnObject(Vector3& spawnPoint);
	void ShowObjectUI();

	void ObjectPicking(Ray& ray);
	void ShowObjectPickingUI();

	void Init(std::shared_ptr<Terrain> owner);
	void Update();
	void Render();

	SpaceDivideTree();
	~SpaceDivideTree();
};

