#pragma once

class SpaceDivideTree;
class Picking;
class HeightMap;
class Splatting;
class MapRenderer;
class Picking;
class ObjectManager;

enum Mode
{
	Mode_Height = 0,
	Mode_Splatting,
	Mode_Object,
	Mode_ObjPicking
};

class Terrain : public Component, public std::enable_shared_from_this<Terrain>
{
private:
	using Base = Component;

	// imgui value
	ImGuiWindowFlags window_flags = 0;
	Mode mode = Mode_Height;
	float brushSize = 10.0f;

	struct MapDataDesc
	{
		float heightScale = 1;
		int rowNum = 129;
		int colNum = 129;
		float devideTreeDepth = 2;
		float cellDistance = 1;
	};
	MapDataDesc mapDataDesc;

public:
	UINT rowNum;
	UINT colNum;
	UINT rowCellNum;
	UINT colCellNum;
	UINT vertexCount;
	UINT faceCount;
	float cellDistance;
	float heightScale;

	int devideTreeDepth = 1;

	std::wstring baseTexturePath;
	std::wstring splattingDataPath;
	std::wstring heightMapFilePath;
	std::wstring sceneFilePath;

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

	void SaveMapData(std::wstring mapDataPath);
	void LoadMapData(std::wstring mapDataPath);

public:
	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void Render() override;

	Terrain();
	~Terrain();
};

