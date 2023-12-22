#pragma once

class SpaceDivideTree;
class Picking;
class HeightMap;
class Splatting;

struct TerrainDesc
{
	UINT rowNum = 0;
	UINT colNum = 0;
	float cellDistance = 0;
	float heightScale = 0;

	std::wstring textureFilePath   = L"";
	std::wstring heightMapFilePath = L"";
	std::wstring shaderFilePath    = L"";

	std::wstring alphaTexPath = L"";

	int DevideTreeDepth = 1;
};

class Terrain : public Component, public std::enable_shared_from_this<Terrain>
{
private:
	using Base = Component;
public:
	UINT rowNum;
	UINT colNum;
	UINT rowCellNum;
	UINT colCellNum;
	UINT vertexCount;
	UINT faceCount;
	float cellDistance;

	int devideTreeDepth = 1;

	std::wstring textureFilePath;		// 그냥 terrain이 들고있게 할지?
	std::wstring shaderFilePath;		// 그냥 terrain이 들고있게 할지?

	std::vector<PNCTVertex> vertices;
	std::vector<UINT> indices;

	std::vector<Vector3> faceNormalList;
	std::vector<int> normalVectorLookTable;

	// height map
	std::shared_ptr<HeightMap> heightMap;

	// Splatting
	std::shared_ptr<Splatting> splatting;

	// quad tree
	std::shared_ptr<SpaceDivideTree> spaceDivideTree;

	// temp : for picking
	int pickingMode = 0;

	// temp : for tilling texture
	int tillingTextureNum = 0;

private:
	// create data
	void CreateVertexData();
	void CreateIndexData();

	// create height map data
	void InitFaceNormal();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
	void CalcFaceNormals();
	Vector3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);

	// calc function
	void CalcVertexColor(Vector3 vLightDir);

private:

	// temp : for picking
	std::vector<UINT> UpdateVertexIdxList;
	float changeHeight = 10.0f;
	float radius = 10.0f;
	void UpdateVertexHeight(Vector3 centerPos);
	void FindChangeVertex(Vector3 centerPos, int pickNodeIdx);

public:
	void Init() override;
	void Update() override;
	void Render() override;

	Terrain(TerrainDesc desc);
	~Terrain();
};

