#pragma once

class SpaceDivideTree;
class Picking;
class HeightMap;
class Splatting;
class MapRenderer;
class Picking;
class ObjectManager;

struct TerrainDesc
{
	UINT rowNum = 0;
	UINT colNum = 0;
	float cellDistance = 0;
	float heightScale = 0;

	std::wstring textureFilePath   = L"";
	std::wstring heightMapFilePath = L"";
	std::wstring sceneFilePath     = L"";

	std::wstring alphaTexPath = L"";

	int DevideTreeDepth = 1;
};

class Terrain : public Component, public std::enable_shared_from_this<Terrain>
{
private:
	using Base = Component;

	// imgui value
	int pickingMode = 0;
	int tillingTextureNum = 0;
	int changeHeightMode = 0;
	float changeHeight = 10.0f;
	float radius = 10.0f;

public:
	UINT rowNum;
	UINT colNum;
	UINT rowCellNum;
	UINT colCellNum;
	UINT vertexCount;
	UINT faceCount;
	float cellDistance;

	int devideTreeDepth = 1;

	std::wstring heightMapFilePath;
	std::wstring alphaTexPath;
	std::wstring sceneFilePath = L"";

	std::vector<PNCTVertex> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> leafNodeIndexList;

	std::vector<Vector3> faceNormalList;
	std::vector<int> normalVectorLookTable;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;

	std::shared_ptr<HeightMap> heightMap;
	std::shared_ptr<Splatting> splatting;
	std::shared_ptr<Picking> picking;
	std::shared_ptr<SpaceDivideTree> spaceDivideTree;

	std::shared_ptr<MapRenderer> mapRenderer;

private:
	// create map data
	void CreateVertexData();
	void CreateIndexData();
	void InitFaceNormal();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
	void CalcFaceNormals();
	Vector3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	void CreateLeafNodeIndexList();

	// calc function
	void CalcVertexColor(Vector3 vLightDir);
	void UpdateVertexHeight(Vector3 centerPos);

public:
	void Init() override;
	void Update() override;
	void Render() override;

	void SaveMapData();
	void LoadMapData();

	Terrain(TerrainDesc desc);
	~Terrain();
};

