#pragma once

struct MapInfo
{
	int columnNum = 0;
	int rowNum = 0;
	float cellDistance = 0.0f;
	float heightScale = 0.0f;

	std::wstring heightMapTextureFileName = L"";
	std::wstring mapTextureFileName = L"";

	Vector3 lightDir = { 0,-1,0 };
};

class HeightMap : public MonoBehaviour
{
private:
	std::vector<PNCTVertex> verteces;
	std::vector<UINT> indices;
	std::vector<Vector3> faceNormalList;
	std::vector<int> normalVecLookUpTable;

	MapInfo mapInfo;

	UINT divisionColumnNum = 0;
	UINT divisionRowNum = 0;

	UINT columnNum = 0;
	UINT rowNum = 0;
	UINT columnCellNum = 0;
	UINT rowCellNum = 0;

	UINT vertexNum = 0;
	UINT faceNum = 0;
	float cellDistance = 0.0f;


	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
public:

private:

public:
	virtual void Init()			override;
	virtual void BeginPlay()	override;
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void Release()		override;

	void GenVertexNormalVec();
	void CalcVertexColor(Vector3 lightDir);
	void InitFaceNormalVec();
	void CalcFaceNormalVec();
	float GetHeightOfVertex(int index);
	Vector3 ComputeFaceNormal(int index0, int index1, int index2);

	void GenNormalVecLookupTable();
	void CalcPerVertexNormalsFastLookup();
	float GetHeightMap(int row, int col);
	Vector3 GetVertexNomal(int index);
	Vector4 GetColorOfVertex(int index);
	Vector2 GetTexOfVertex(float offsetX, float offsetY);
	float GetHeight(float x, float z);
	float Lerp(float start, float end, float tangent);

	bool Set(MapInfo& info);
	void SetMatrix();
};

