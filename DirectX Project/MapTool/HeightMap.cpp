#include "pch.h"
#include "HeightMap.h"

void HeightMap::Init()
{
	//create texture
	mapTexture = std::make_shared<Texture>();
	mapTexture->Load(mapInfo.mapTextureFileName);

	//set height map
	CreateHeightMap(mapInfo.heightMapTextureFileName);

	//set vertex
	vertices.resize(vertexNum);

	float halfCol = (columnCellNum) * 0.5f;
	float halfRow = (rowCellNum) * 0.5f;
	float texOffsetU = 1.0f / (columnCellNum);
	float texOffsetV = 1.0f / (rowCellNum);

	for (int row = 0; row < rowNum; ++row)
	{
		for (int col = 0; col < columnNum; ++col)
		{
			int index = row * columnNum + col;

			vertices[index].position.x = (col - halfCol) * cellDistance;
			vertices[index].position.z = -((row - halfRow) * cellDistance);
			vertices[index].position.y = GetHeightOfVertex(index);

			vertices[index].normal = GetVertexNomal(index);
			vertices[index].color = GetColorOfVertex(index);

			vertices[index].uv = GetTexOfVertex(texOffsetU * col, texOffsetV * row);
		}
	}

	//set index
	indices.resize(faceNum * 3);

	UINT index = 0;
	for (int row = 0; row < rowCellNum; ++row)
	{
		for (int col = 0; col < columnCellNum; ++col)
		{
			int nextRow = row + 1;
			int nextCol = col + 1;

			indices[index + 0] = row * columnNum + col;
			indices[index + 1] = row * columnNum + nextCol;
			indices[index + 2] = nextRow * columnNum + col;

			indices[index + 3] = indices[index + 2];
			indices[index + 4] = indices[index + 1];
			indices[index + 5] = nextRow * columnNum + nextCol;

			index += 6;
		}
	}

	GenVertexNormalVec();

	//make vertex buffer
	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vertices);
	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(indices);
}

void HeightMap::BeginPlay()
{
}

void HeightMap::FixedUpdate()
{
}

void HeightMap::Update()
{
	globalDesc.View = Camera::viewMatrix;
	globalDesc.Projection = Camera::projectionMatrix;
	globalDesc.VirwProjection = Camera::viewMatrix * Camera::projectionMatrix;
	globalDesc.ViewInverse = Camera::viewMatrix.Invert();
	globalBuffer->CopyData(globalDesc);
	globalEffectBuffer->SetConstantBuffer(globalBuffer->GetConstantBuffer().Get());


	transformDesc.World = GetTransform()->GetWorldMatrix();
	transformBuffer->CopyData(transformDesc);
	transformEffectBuffer->SetConstantBuffer(transformBuffer->GetConstantBuffer().Get());
}

void HeightMap::PostUpdate()
{
}

void HeightMap::PreRender()
{
}

void HeightMap::Render()
{
	shader->GetSRV("Texture0")->SetResource(mapTexture->GetShaderResourceView().Get());

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, indexBuffer->GetIndexCount());
}

void HeightMap::PostRender()
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
}

void HeightMap::CalcVertexColor(Vector3 lightDir)
{
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < columnNum; iCol++)
		{
			int  iVertexIndex = iRow * columnNum + iCol;

			float fDot = (mapInfo.lightDir * -1).Dot(vertices[iVertexIndex].normal);

			vertices[iVertexIndex].color *= fDot;
			vertices[iVertexIndex].color.w = 1.0f;
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
	return heightList[index];
}

Vector3 HeightMap::ComputeFaceNormal(int index0, int index1, int index2)
{
	Vector3 vNormal;
	Vector3 v0 = vertices[index1].position - vertices[index0].position;
	Vector3 v1 = vertices[index2].position - vertices[index0].position;

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
			triIndex = normalVecLookUpTable[i * 6 + j];

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

	CalcVertexColor(mapInfo.lightDir);
}

float HeightMap::GetHeightMap(int row, int col)
{
	return vertices[row * rowNum + col].position.y;
}

Vector3 HeightMap::GetVertexNomal(int index)
{
	return Vector3(0, 1, 0);
}

Vector4 HeightMap::GetColorOfVertex(int index)
{
	return Vector4(1,1,1,1);
}

Vector2 HeightMap::GetTexOfVertex(float offsetX, float offsetY)
{
	return Vector2(offsetX, offsetY);
}

/// <summary>
/// may not be used
/// </summary>
/// <param name="x"></param>
/// <param name="z"></param>
/// <returns></returns>
float HeightMap::GetHeight(float x, float z)
{
	float fCellX = (float)(columnCellNum * cellDistance / 2.0f + x);
	float fCellZ = (float)(rowNum * cellDistance / 2.0f - z);

	fCellX /= (float)cellDistance;
	fCellZ /= (float)cellDistance;

	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(columnNum - 2) < fVertexCol)	fVertexCol = (float)(columnNum - 2);
	if ((float)(rowNum - 2) < fVertexRow)	fVertexRow = (float)(rowNum - 2);

	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;


	float fHeight = 0.0f;
	if (fDeltaZ < (1.0f - fDeltaX))
	{
		float uy = B - A;
		float vy = C - A;
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}

	return fHeight;
}

float HeightMap::Lerp(float start, float end, float tangent)
{
	return start - (start * tangent) + (end * tangent);
}

void HeightMap::CreateHeightMap(std::wstring heightMapTextureFile)
{
	heightMapTexture = std::make_shared<Texture>();
	heightMapTexture->Load(heightMapTextureFile);

	Vector2 size = heightMapTexture->GetSize();
	auto info = heightMapTexture->GetInfo();
	auto mdata = info->GetMetadata();
	uint8_t* pixelBuffer = info->GetPixels();

	heightList.resize(mdata.height * mdata.width);

	for (int row = 0; row < mdata.height; ++row)
	{
		for (int col = 0; col < mdata.width; ++col)
		{
			int index = row * mdata.width + col;
			heightList[index] = pixelBuffer[index * 4]; 
		}
	}

	columnNum = mdata.width;
	rowNum = mdata.height;
	columnCellNum = columnNum - 1;
	rowCellNum = rowNum - 1;

	vertexNum = columnNum * rowNum;
	faceNum = columnCellNum * rowCellNum * 2;
}

void HeightMap::Set(MapInfo& info)
{
	this->mapInfo = info;

	columnNum = info.columnNum;
	rowNum = info.rowNum;
	columnCellNum = columnNum - 1;
	rowCellNum = rowNum - 1;

	vertexNum = columnNum * rowNum;
	faceNum = columnCellNum * rowCellNum * 2;
	cellDistance = info.cellDistance;
}
