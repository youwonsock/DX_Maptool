#pragma once

struct MapInfo
{
	float cellDistance = 0.0f;

	UINT columnNum = 0;
	UINT rowNum = 0;

	std::wstring heightMapTextureFileName = L"";
	std::wstring mapTextureFileName = L"";

	Vector3 lightDir = { 0,-1,0 };
};

class HeightMap : public MonoBehaviour
{
private:
	std::vector<PNCTVertex> vertices;
	std::vector<UINT> indices;
	std::vector<Vector3> faceNormalList;
	std::vector<int> normalVecLookUpTable;
	std::vector<float> heightList;

	MapInfo mapInfo;

	UINT columnNum = 0;
	UINT rowNum = 0;
	UINT columnCellNum = 0;
	UINT rowCellNum = 0;

	UINT vertexNum = 0;
	UINT faceNum = 0;
	float cellDistance = 0.0f;


	// render
	std::shared_ptr<Texture> heightMapTexture = nullptr;
	std::shared_ptr<Texture> mapTexture = nullptr;
	std::shared_ptr<Shader> shader;

	std::shared_ptr<ConstantBuffer<GlobalDesc>> globalBuffer;
	std::shared_ptr<ConstantBuffer<TransformDesc>> transformBuffer;

	ComPtr<ID3DX11EffectConstantBuffer> globalEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> transformEffectBuffer;

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	GlobalDesc globalDesc;
	TransformDesc transformDesc;
public:

private:
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

	//height map function
	void CreateHeightMap(std::wstring heightMapTextureFile);

public:
	virtual void Init()			override;
	virtual void BeginPlay()	override;
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render()		override;
	virtual void PostRender()	override;
	virtual void Release()		override;

	void Set(MapInfo& info);

	HeightMap(std::shared_ptr<Shader>& shader)
	{
		this->shader = shader;

		globalBuffer = std::make_shared<ConstantBuffer<GlobalDesc>>(Global::g_device, Global::g_immediateContext);
		globalBuffer->Create();
		globalEffectBuffer = shader->GetConstantBuffer("GlobalBuffer");

		transformBuffer = std::make_shared<ConstantBuffer<TransformDesc>>(Global::g_device, Global::g_immediateContext);
		transformBuffer->Create();
		transformEffectBuffer = shader->GetConstantBuffer("TransformBuffer");
	}

	~HeightMap() {};
};

