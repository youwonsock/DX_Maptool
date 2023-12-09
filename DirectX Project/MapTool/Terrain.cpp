#include "pch.h"
#include "Terrain.h"
#include "SpaceDivideTree.h"

Terrain::Terrain(TerrainDesc desc) : Base(ComponentType::Terrain)
{
	rowNum = desc.rowNum;
	colNum = desc.colNum;
	rowCellNum = rowNum - 1;
	colCellNum = colNum - 1;
	vertexCount = rowNum * colNum;
	faceCount = rowCellNum * colCellNum * 2;
	cellDistance = desc.cellDistance;
	heightScale = desc.heightScale;

	if (desc.textureFilePath.length() < 1 || desc.shaderFilePath.length() < 1)
	{
		assert(false);
	}

	textureFilePath = desc.textureFilePath;
	shaderFilePath = desc.shaderFilePath;

	if (desc.heightMapFilePath.length() > 0)
	{
		heightMapFilePath = desc.heightMapFilePath;
		useHeightMap = true;
	}
}

Terrain::~Terrain()
{
}

// -------------------------------------------------------------------------------
// ------------------------------component functions -----------------------------
// -------------------------------------------------------------------------------

void Terrain::Init()
{
	CreateVertexData();
	CreateIndexData();

	// GenerateVertexNormal();
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();

	// quad tree
	spaceDivideTree = std::make_shared<SpaceDivideTree>(shared_from_this());
	spaceDivideTree->Init();
}

void Terrain::Update()
{
	spaceDivideTree->Update();
}

void Terrain::Render()
{
	spaceDivideTree->Render();

}

// -------------------------------------------------------------------------------
// ------------------------------private functions -------------------------------
// -------------------------------------------------------------------------------

void Terrain::CreateVertexData()
{
	if (useHeightMap)
		CreateHeightMapData();

	vertices.resize(vertexCount);

	float fOffSetU = 1.0f / (colNum - 1);
	float fOffSetV = 1.0f / (rowNum - 1);
	float fHalfCols = (colCellNum) * 0.5f;
	float fHalfRows = (rowCellNum) * 0.5f;

	float tY;
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < colNum; iCol++)
		{
			int  iVertexIndex = iRow * colNum + iCol;

			vertices[iVertexIndex].position.x = (iCol - fHalfCols) * cellDistance;
			vertices[iVertexIndex].position.z = -((iRow - fHalfRows) * cellDistance);

			tY = useHeightMap ? GetHeightVertex(iVertexIndex) : 0.0f;
			vertices[iVertexIndex].position.y = tY;

			vertices[iVertexIndex].normal = Vector3(0,1,0);
			vertices[iVertexIndex].color = Vector4(1, 1, 1, 1);
			vertices[iVertexIndex].uv = Vector2(iCol * fOffSetU, iRow * fOffSetV);
		}
	}
}

void Terrain::CreateIndexData()
{
	indices.resize(faceCount * 3);

	int iIndex = 0;
	for (int iRow = 0; iRow < rowCellNum; ++iRow)
	{
		for (int iCol = 0; iCol < colCellNum; ++iCol)
		{
			int nextCol = iCol + 1;
			int nextRow = iRow + 1;

			indices[iIndex + 0] = iRow * colNum + iCol;
			indices[iIndex + 1] = iRow * colNum + nextCol;
			indices[iIndex + 2] = nextRow * colNum + iCol;

			indices[iIndex + 3] = indices[iIndex + 2];
			indices[iIndex + 4] = indices[iIndex + 1];
			indices[iIndex + 5] = nextRow * colNum + nextCol;

			iIndex += 6;
		}
	}
}




void Terrain::CreateHeightMapData()
{
	std::unique_ptr<Texture> heightMap = std::make_unique<Texture>();
	heightMap->Load(heightMapFilePath);

	Vector2 size = heightMap->GetSize();
	auto& info = heightMap->GetInfo();
	auto mData = info->GetMetadata();
	auto images = info->GetImages();

	if (!CheckSquare(mData.width - 1))
		mData.width = ResizeMap(mData.width);
	if (!CheckSquare(mData.height - 1))
		mData.height = ResizeMap(mData.height);

	rowNum = mData.height;
	colNum = mData.width;
	rowCellNum = rowNum - 1;
	colCellNum = colNum - 1;
	vertexCount = rowNum * colNum;
	faceCount = rowCellNum * colCellNum * 2;

	heightList.resize(rowNum * colNum);
	UCHAR* pTexels = (UCHAR*)images->pixels;

	for (UINT i = 0; i < rowNum; i++)
	{
		UINT rowStart = i * images->rowPitch;
		for (UINT j = 0; j < colNum; j++)
		{
			UINT colStart = j * 4;
			UINT uRed = pTexels[rowStart + colStart + 0];
			heightList[i * mData.width + j] = (float)uRed * 0.1;
		}
	}
}

void Terrain::InitFaceNormal()
{
	faceNormalList.resize(faceCount);

	for (int i = 0; i < faceCount; i++)
		faceNormalList[i] = Vector3(0, 0, 0);
}

void Terrain::GenNormalLookupTable()
{
	if (normalVectorLookTable.size() > 0)
	{
		normalVectorLookTable.clear();
		normalVectorLookTable.shrink_to_fit();
	}
	UINT buffersize = rowNum * colNum * 6;
	normalVectorLookTable.resize(buffersize);

	for (int i = 0; i < buffersize; i++)
		normalVectorLookTable[i] = -1;

	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				UINT vertex = indices[i * 3 + j];
				if (normalVectorLookTable[vertex * 6 + k] == -1)
				{
					normalVectorLookTable[vertex * 6 + k] = i;
					break;
				}
			}
		}
	}
}

void Terrain::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormals();

	int j = 0;
	for (int i = 0; i < vertexCount; i++)
	{
		Vector3 avgNormal;
		avgNormal = Vector3(0.0f, 0.0f, 0.0f);

		for (j = 0; j < 6; j++)
		{
			int triIndex;
			triIndex = normalVectorLookTable[i * 6 + j];

			if (triIndex != -1)
				avgNormal += faceNormalList[triIndex];
			else
				break;
		}

		_ASSERT(j > 0);
		avgNormal.x /= (float)j;
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		avgNormal.Normalize();

		vertices[i].normal = avgNormal;
	}

	CalcVertexColor(Vector3(0,-1,0));
}

void Terrain::CalcFaceNormals()
{
	int j = 0;
	for (int i = 0; i < faceCount * 3; i += 3)
	{
		DWORD i0 = indices[i];
		DWORD i1 = indices[i + 1];
		DWORD i2 = indices[i + 2];
		faceNormalList[j] = ComputeFaceNormal(i0, i1, i2);
		++j;
	}
}

Vector3 Terrain::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	Vector3 vNormal;
	Vector3 v0 = vertices[dwIndex1].position - vertices[dwIndex0].position;
	Vector3 v1 = vertices[dwIndex2].position - vertices[dwIndex0].position;

	v0.Cross(v1, vNormal);
	vNormal.Normalize();

	return vNormal;
}

bool Terrain::CheckSquare(int n)
{
	return 	(n & (n - 1)) == 0;
}

int Terrain::ResizeMap(int n)
{
	int result = 0;
	int N = 2;

	while (N < n)
	{
		result = N;
		N *= 2;
	}

	return result + 1;
}




void Terrain::CalcVertexColor(Vector3 vLightDir)
{
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < colNum; iCol++)
		{

			int  iVertexIndex = iRow * colNum + iCol;

			float fDot = vLightDir.Dot(vertices[iVertexIndex].normal);

			vertices[iVertexIndex].color *= fDot;
			vertices[iVertexIndex].color.w = 1.0f;
		}
	}
}

float Terrain::GetHeightMap(int row, int col)
{
	return heightList[row * rowNum + col] * heightScale;
}

float Terrain::GetHeightVertex(UINT index)
{
	return heightList[index] * heightScale;
}