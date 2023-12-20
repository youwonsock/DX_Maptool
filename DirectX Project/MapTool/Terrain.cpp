#include "pch.h"
#include "Terrain.h"

#include "SpaceDivideTree.h"
#include "SectionNode.h"

#include "Picking.h"
#include <fstream>

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

	devideTreeDepth = desc.DevideTreeDepth;

	if (desc.textureFilePath.length() < 1 || desc.shaderFilePath.length() < 1)
	{
		assert(false);
	}

	textureFilePath = desc.textureFilePath;
	shaderFilePath = desc.shaderFilePath;

	if (desc.heightMapFilePath.length() > 0)
	{
		heightTexPath = desc.heightMapFilePath;
	}

	if(desc.alphaTexPath.length() > 0)
	{
		alphaTexPath = desc.alphaTexPath;
	}

	// temp : for picking
	{
		picking = std::make_shared<Picking>();
	}

	// temp : for tilling
	{
		texture1 = std::make_shared<Texture>();
		texture1->Load(L"../../Res/Textures/Terrain/Red.PNG");
		
		texture2 = std::make_shared<Texture>();
		texture2->Load(L"../../Res/Textures/Terrain/Green.PNG");
	
		texture3 = std::make_shared<Texture>();
		texture3->Load(L"../../Res/Textures/Terrain/Blue.PNG");

		texture4 = std::make_shared<Texture>();
		texture4->Load(L"../../Res/Textures/Terrain/White.PNG");

	}

	CreateHeightMapData();

	alphaTexture = std::make_shared<Texture>();
	if(!alphaTexture->Load(alphaTexPath))
		alphaTexture->CreateTexture(rowNum, colNum);
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
	spaceDivideTree->maxDepth = devideTreeDepth;
	spaceDivideTree->Init();
}

void Terrain::Update()
{
	// temp : for picking
	{
		ImGui::InputFloat("Change Height", &changeHeight);
		ImGui::InputFloat("Radius", &radius);

		ImGui::InputInt("Picking Mode", &pickingMode);
		ImGui::InputInt("Tilling Texture", &tillingTextureNum);

		if(pickingMode < 0 || pickingMode > 1)
			pickingMode = 0;
		if(tillingTextureNum < 0 || tillingTextureNum > 4)
			tillingTextureNum = 0;

		if (InputManager::GetInstance().GetMouseState(0) > KeyState::UP)
		{
			picking->UpdateRay();

			// find leaf node collision with ray
			std::vector<std::shared_ptr<SectionNode>> sectionList;
			for (int i = 0; i < spaceDivideTree->leafNodeMap.size(); ++i)
			{
				if(Collision::CubeToRay(spaceDivideTree->leafNodeMap[i]->boundingBox, picking->GetRay()))
					sectionList.push_back(spaceDivideTree->leafNodeMap[i]);
			}

			// find face collision with ray
			int i = 0;
			int pickNodeIdx = 0;
			Vector3 pickPoint;
			bool findPickPoint = false;

			auto& leafNodeIdxList = spaceDivideTree->leafNodeIndexList;

			while (!findPickPoint)
			{
				if (i < sectionList.size())
				{
					auto& leafVertices = sectionList[i]->vertices;
					for(int j = 0; j < leafNodeIdxList.size(); j += 3)
					{
						Vector3 v0 = leafVertices[leafNodeIdxList[j + 0]].position;
						Vector3 v1 = leafVertices[leafNodeIdxList[j + 1]].position;
						Vector3 v2 = leafVertices[leafNodeIdxList[j + 2]].position;

						if (Collision::RayToFace(picking->GetRay(), v0, v1, v2, &pickPoint))
						{
							findPickPoint = true;
							pickNodeIdx = sectionList[i]->nodeIndex;

							break;
						}
					}
				}
				else
					break;

				++i;
			}
			if (!findPickPoint)
				return;

			FindChangeVertex(pickPoint, pickNodeIdx);

			switch (pickingMode)
			{
			case(0):
				UpdateVertexHeight(pickPoint);
				break;
			case(1):
				TillingTexture(pickPoint);
				break;
			default:
				break;
			}

			// update vertex buffer
			spaceDivideTree->UpdateVertex();
				
		}

		SaveHeightMap();
	}


	spaceDivideTree->Update();
}

void Terrain::Render()
{
	spaceDivideTree->Render();
}

void Terrain::SaveHeightMap()
{
	if (InputManager::GetInstance().GetKeyState(DIK_M) == KeyState::PUSH)
	{
		std::vector<BYTE> heightList;
		heightList.resize(rowNum * colNum * 4);

		for (int i = 0; i < rowNum * colNum; ++i)
		{
			heightList[i * 4 + 0] = vertices[i].position.y;
			heightList[i * 4 + 1] = vertices[i].position.y;
			heightList[i * 4 + 2] = vertices[i].position.y;
			heightList[i * 4 + 3] = vertices[i].position.y;
		}

		heightMap->UpdateTexture(heightList);

		heightMap->SaveTexture(heightTexPath);
		alphaTexture->SaveTexture(alphaTexPath);
	}
}


// -------------------------------------------------------------------------------
// ----------------------------- tilling function -------------------------------
// -------------------------------------------------------------------------------


void Terrain::TillingTexture(Vector3 centerPos)
{
	float distance = 0.0f;
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	for (UINT i : UpdateVertexIdxList)
	{
		distance = (vertices[i].position - centerPos).Length();

		if (distance < 1.0f)
			distance = 1.0;

		distance = (1 / distance);
		distance *= 100.0f;

		switch (tillingTextureNum)
		{
		case(0):
			vertices[i].color.x += distance;
			vertices[i].color.x = std::clamp(vertices[i].color.x, 0.0f, 255.0f);
			break;
		case(1):
			vertices[i].color.y += distance;
			vertices[i].color.y = std::clamp(vertices[i].color.y, 0.0f, 255.0f);
			break;
		case(2):
			vertices[i].color.z += distance;
			vertices[i].color.z = std::clamp(vertices[i].color.z, 0.0f, 255.0f);
			break;
		case(3):
			vertices[i].color.w += distance;
			vertices[i].color.w = std::clamp(vertices[i].color.w, 0.0f, 255.0f);
			break;
		case(4):
			vertices[i].color.x = 0;
			vertices[i].color.y = 0;
			vertices[i].color.z = 0;
			vertices[i].color.w = 0;
			break;
		default:
			break;
		}
	}

	//temp : update alpha texture
	std::vector<BYTE> colorList;
	colorList.resize(rowNum * colNum * 4);

	for (int i = 0; i < rowNum * colNum; ++i)
	{
		colorList[i * 4 + 0] = vertices[i].color.x;
		colorList[i * 4 + 1] = vertices[i].color.y;
		colorList[i * 4 + 2] = vertices[i].color.z;
		colorList[i * 4 + 3] = vertices[i].color.w;
	}

	alphaTexture->UpdateTexture(colorList);
}

// -------------------------------------------------------------------------------
// ----------------------------- pickking function -------------------------------
// -------------------------------------------------------------------------------

void Terrain::UpdateVertexHeight(Vector3 centerPos)
{
	float distance = 0.0f;
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	for (UINT i : UpdateVertexIdxList)
	{
		distance = (vertices[i].position - centerPos).Length();

		if (distance < 1.0f)
			distance = 1.0;
		
		distance = (1 / distance);
		distance = cosf(distance * 3.141592f / 180.0f);

		vertices[i].position.y += (distance * changeHeight * deltaTime);
		heightList[i] = vertices[i].position.y / heightScale;
	}
}

void Terrain::FindChangeVertex(Vector3 centerPos, int pickNodeIdx)
{
	UpdateVertexIdxList.clear();

	// 0 : right, 1 : left, 2 : bottom, 3 : top
	// find node by node idx
	DWORD LT, RT, LB, RB;
	std::shared_ptr<SectionNode> pNode = spaceDivideTree->leafNodeMap[pickNodeIdx];

	// lt, lb
	{
		while (true)
		{
			if (pNode->neighborNodeList[1] == nullptr)
				break;

			auto& ltConerPos = vertices[pNode->cornerIndexList[0]].position;

			if (centerPos.x - radius < ltConerPos.x)
				pNode = pNode->neighborNodeList[1];
			else
				break;
		}

		std::shared_ptr<SectionNode> ltNode = pNode;
		std::shared_ptr<SectionNode> lbNode = pNode;

		// top
		while (true)
		{
			if (ltNode->neighborNodeList[3] == nullptr)
				break;

			auto& ltConerPos = vertices[ltNode->cornerIndexList[0]].position;

			if (centerPos.z + radius > ltConerPos.z)
				ltNode = ltNode->neighborNodeList[3];
			else
				break;
		}
		LT = ltNode->cornerIndexList[0];

		// bottom
		while (true)
		{
			if (lbNode->neighborNodeList[2] == nullptr)
				break;

			auto& lbConerPos = vertices[lbNode->cornerIndexList[2]].position;

			if (centerPos.z - radius < lbConerPos.z)
				lbNode = lbNode->neighborNodeList[2];
			else
				break;
		}
		LB = lbNode->cornerIndexList[2];
	}


	// rt, rb
	{
		pNode = spaceDivideTree->leafNodeMap[pickNodeIdx];

		while (true)
		{
			if (pNode->neighborNodeList[0] == nullptr)
				break;

			auto& rtConerPos = vertices[pNode->cornerIndexList[1]].position;

			if (centerPos.x + radius > rtConerPos.x)
				pNode = pNode->neighborNodeList[0];
			else
				break;
		}

		std::shared_ptr<SectionNode> rtNode = pNode;
		std::shared_ptr<SectionNode> rbNode = pNode;

		// top
		while (true)
		{
			if (rtNode->neighborNodeList[3] == nullptr)
				break;

			auto& rtConerPos = vertices[rtNode->cornerIndexList[1]].position;

			if (centerPos.z + radius > rtConerPos.z)
				rtNode = rtNode->neighborNodeList[3];
			else
				break;
		}
		RT = rtNode->cornerIndexList[1];

		// bottom
		while (true)
		{
			if (rbNode->neighborNodeList[2] == nullptr)
				break;

			auto& rbConerPos = vertices[rbNode->cornerIndexList[3]].position;

			if (centerPos.z - radius < rbConerPos.z)
				rbNode = rbNode->neighborNodeList[2];
			else
				break;
		}
		RB = rbNode->cornerIndexList[3];
	}

	// loop (check find node area)
	Circle circle = Circle(Vector2(centerPos.x, centerPos.z), radius);

	int iNumCols = colNum;
	int iStartRow = LT / iNumCols;
	int iEndRow = LB / iNumCols;
	int iStartCol = LT % iNumCols;
	int iEndCol = RT % iNumCols; 

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;

	int iIndex = 0;
	for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * iNumCols + iCol;

			if(Collision::CircleToPoint(circle, vertices[iCurrentIndex].position))
				UpdateVertexIdxList.push_back(iCurrentIndex);
		}
	}
}

// -------------------------------------------------------------------------------
// ------------------------------private functions -------------------------------
// -------------------------------------------------------------------------------

void Terrain::CreateVertexData()
{
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

			tY = GetHeightVertex(iVertexIndex);
			vertices[iVertexIndex].position.y = tY;

			vertices[iVertexIndex].normal = Vector3(0,1,0);
			vertices[iVertexIndex].color = Vector4(0, 0, 0, 0);
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
	heightMap = std::make_unique<Texture>();
	if (heightMap->Load(heightTexPath))
	{
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
				heightList[i * mData.width + j] = (float)uRed;
			}
		}
	}
	else
	{
		heightMap->CreateTexture(rowNum, colNum);

		rowNum = rowNum;
		colNum = colNum;
		rowCellNum = rowNum - 1;
		colCellNum = colNum - 1;
		vertexCount = rowNum * colNum;
		faceCount = rowCellNum * colCellNum * 2;

		heightList.resize(rowNum * colNum);

		for (UINT i = 0; i < heightList.size(); i++)
			heightList[i] = 0.0f;
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
			vertices[iVertexIndex].color.w = 0.0f;
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