#include "pch.h"
#include "HeightMap.h"

void HeightMap::Init()
{
}

void HeightMap::BeginPlay()
{
}

void HeightMap::FixedUpdate()
{
}

void HeightMap::Update()
{
}

void HeightMap::PostUpdate()
{
}

void HeightMap::Release()
{
}

void HeightMap::GenVertexNormalVec()
{
	InitFaceNormalVec();
	GenNormalVecLookupTable();
	CalcPerVertexNormalsFastLookup();

	// create buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(PNCTVertex) * vertexNum;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = verteces.data();

		HRESULT hr = Global::g_device->CreateBuffer(&desc, &data, vertexBuffer.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			ShowErrorMessage(hr);
	}
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices.data();

		HRESULT hr = Global::g_device->CreateBuffer(&desc, &data, indexBuffer.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			ShowErrorMessage(hr);
	}
}

void HeightMap::CalcVertexColor(Vector3 lightDir)
{
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < columnNum; iCol++)
		{
			int  iVertexIndex = iRow * columnNum + iCol;

			float fDot = (mapInfo.lightDir * -1).Dot(verteces[iVertexIndex].normal);

			verteces[iVertexIndex].color *= fDot;
			verteces[iVertexIndex].color.w = 1.0f;
		}
	}
}

void HeightMap::InitFaceNormalVec()
{
	faceNormalList.resize(faceNum);

	for (int i = 0; i < faceNum; ++i)
		faceNormalList[i] = Vector3(0, 0, 0);
}

void HeightMap::CalcFaceNormalVec()
{
	int j = 0;
	for (int i = 0; i < faceNum * 3; i += 3)
	{
		DWORD i0 = indices[i];
		DWORD i1 = indices[i + 1];
		DWORD i2 = indices[i + 2];

		faceNormalList[j] = ComputeFaceNormal(i0, i1, i2);
		++j;
	}
}

float HeightMap::GetHeightOfVertex(int index)
{
	return 0.0f;
}

Vector3 HeightMap::ComputeFaceNormal(int index0, int index1, int index2)
{
	Vector3 vNormal;
	Vector3 v0 = verteces[index1].position - verteces[index0].position;
	Vector3 v1 = verteces[index2].position - verteces[index0].position;

	vNormal.Cross(v0, v1);
	vNormal.Normalize();

	return vNormal;
}

void HeightMap::GenNormalVecLookupTable()
{
	int vertexCount = rowNum * columnNum * 6;

	if (normalVecLookUpTable.size() > 1)
		normalVecLookUpTable.clear();

	normalVecLookUpTable.resize(vertexCount);

	for (int i = 0; i < vertexCount; i++)
		normalVecLookUpTable[i] = -1;

	for (int i = 0; i < faceNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				int vertex = indices[i * 3 + j];

				if (normalVecLookUpTable[vertex * 6 + k] == -1)
				{
					normalVecLookUpTable[vertex * 6 + k] = i;
					break;
				}
			}
		}
	}
}

void HeightMap::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormalVec();

	int j = 0;
	for (int i = 0; i < vertexNum; i++)
	{
		Vector3 avgNormal;
		avgNormal = Vector3(0.0f, 0.0f, 0.0f);

		for (j = 0; j < 6; j++)
		{
			int triIndex;
			triIndex = normalVecLookupTable[i * 6 + j];

			if (triIndex != -1)
			{
				avgNormal += faceNormalList[triIndex];
			}
			else
				break;
		}

		_ASSERT(j > 0);

		avgNormal.x /= (float)j;
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		avgNormal.Normalize();

		vertices[i].normal.x = avgNormal.x;
		vertices[i].normal.y = avgNormal.y;
		vertices[i].normal.z = avgNormal.z;

	}

	CalcVertexColor(info.lightDir);
}

float HeightMap::GetHeightMap(int row, int col)
{
	return 0.0f;
}

Vector3 HeightMap::GetVertexNomal(int index)
{
	return Vector3();
}

Vector4 HeightMap::GetColorOfVertex(int index)
{
	return Vector4();
}

Vector2 HeightMap::GetTexOfVertex(float offsetX, float offsetY)
{
	return Vector2();
}

float HeightMap::GetHeight(float x, float z)
{
	return 0.0f;
}

float HeightMap::Lerp(float start, float end, float tangent)
{
	return 0.0f;
}

bool HeightMap::Set(MapInfo& info)
{
	return false;
}

void HeightMap::SetMatrix()
{
}
