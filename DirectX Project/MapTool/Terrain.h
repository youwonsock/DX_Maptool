#pragma once

class SpaceDivideTree;

struct TerrainDesc
{
	UINT rowNum = 0;
	UINT colNum = 0;
	float cellDistance = 0;
	float heightScale = 0;

	std::wstring textureFilePath   = L"";
	std::wstring heightMapFilePath = L"";
	std::wstring shaderFilePath    = L"";
};

class Terrain : public Component, public std::enable_shared_from_this<Terrain>
{
private:
	using Base = Component;
public:
	bool useHeightMap = false;
	
	UINT rowNum;
	UINT colNum;
	UINT rowCellNum;
	UINT colCellNum;
	UINT vertexCount;
	UINT faceCount;
	float cellDistance;
	float heightScale;

	std::wstring textureFilePath;
	std::wstring heightMapFilePath;	
	std::wstring shaderFilePath;

	std::vector<PNCTVertex> vertices;
	std::vector<UINT> indices;

	std::vector<float> heightList;
	std::vector<Vector3> faceNormalList;
	std::vector<int> normalVectorLookTable;

	// quad tree
	std::shared_ptr<SpaceDivideTree> spaceDivideTree;

private:
	// create data
	void CreateVertexData();
	void CreateIndexData();

	// create height map data
	void CreateHeightMapData();
	void InitFaceNormal();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
	void CalcFaceNormals();
	Vector3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);

	// change height map size to power of 2 + 1
	bool CheckSquare(int n);
	int ResizeMap(int n);

	// calc function
	void CalcVertexColor(Vector3 vLightDir);
	float GetHeightMap(int row, int col);
	float GetHeightVertex(UINT index);

public:

	void Init() override;
	void Update() override;
	void Render() override;

	Terrain(TerrainDesc desc);
	~Terrain();
};

