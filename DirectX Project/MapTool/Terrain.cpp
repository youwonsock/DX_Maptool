#include "pch.h"
#include "Terrain.h"

#include "SpaceDivideTree.h"
#include "SectionNode.h"

#include "HeightMap.h"
#include "Splatting.h"
#include "RenderMgr.h"
#include "Picking.h"

#include <fstream>

Terrain::Terrain(TerrainDesc desc) : Base(ComponentType::Terrain)
{
	if (desc.textureFilePath.length() < 1 || desc.shaderFilePath.length() < 1)
		assert(false);

	devideTreeDepth = desc.DevideTreeDepth;
	cellDistance = desc.cellDistance;

	// shader, texture, (temp)renderMgr
	{
		shader = std::make_shared<Shader>(desc.shaderFilePath);
		texture = std::make_shared<Texture>();
		texture->Load(desc.textureFilePath);
		picking = std::make_shared<Picking>();

		// temp
		renderMgr = std::make_shared<RenderMgr>();
		renderMgr->Init(shader);
	}


	// height map
	{
		heightMap = std::make_shared<HeightMap>();
		
		rowNum = desc.rowNum;
		colNum = desc.colNum;

		heightMap->Init(rowNum, colNum, desc.heightScale, desc.heightMapFilePath);

		// after height map create rowNum, colNum is power of 2 + 1
		rowCellNum = rowNum - 1;
		colCellNum = colNum - 1;
		vertexCount = rowNum * colNum;
		faceCount = rowCellNum * colCellNum * 2;
	}


	// splatting
	{
		SplattingDesc splattingDesc;
		splattingDesc.rowNum = rowNum;
		splattingDesc.colNum = colNum;
		splattingDesc.alphaTexPath = desc.heightMapFilePath.length() < 1 ? L"" : desc.alphaTexPath;
		splattingDesc.texture1Path = L"../../Res/Textures/Terrain/Red.PNG";
		splattingDesc.texture2Path = L"../../Res/Textures/Terrain/Green.PNG";
		splattingDesc.texture3Path = L"../../Res/Textures/Terrain/Blue.PNG";
		splattingDesc.texture4Path = L"../../Res/Textures/Terrain/White.PNG";

		splatting = std::make_shared<Splatting>();
		splatting->Init(splattingDesc);
	}


	// temp (object spawn)
	{
		model = std::make_shared<Model>();
		model->ReadModel(L"Tower/Tower");
		model->ReadMaterial(L"Tower/Tower");
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

	// GenerateVertexNormal, make normal table;
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();

	// quad tree
	spaceDivideTree = std::make_shared<SpaceDivideTree>(shared_from_this());
	spaceDivideTree->maxDepth = devideTreeDepth;
	spaceDivideTree->Init();

	// init color
	splatting->SetVertexByTexture(vertices);
	
	// create leaf node index list(for picking)
	CreateLeafNodeIndexList();


	// temp (object spawn)
	{
		objectShader = std::make_shared<Shader>(L"23. RenderDemo.fx");

		RenderManager::GetInstance().Init(objectShader);
	}
}

void Terrain::Update()
{
	ImGui::InputInt("Change Mode", &changeHeightMode);
	ImGui::InputFloat("Change Height", &changeHeight);
	ImGui::InputFloat("Radius", &radius);

	ImGui::InputInt("Picking Mode", &pickingMode);
	ImGui::InputInt("Tilling Texture", &tillingTextureNum);

	if (pickingMode < 0 || pickingMode > 2)
		pickingMode = 0;
	if (tillingTextureNum < 0 || tillingTextureNum > 4)
		tillingTextureNum = 0;

	// temp : for picking
	{
		if (InputManager::GetInstance().GetMouseState(0) > KeyState::UP)
		{
			auto& ray = CameraManager::GetInstance().GetMainCamera()->GetRay();
			Vector3 pickPoint;

			bool isFind = false;
			std::shared_ptr<SectionNode>& pickNode = picking->FindPickFace(ray, leafNodeIndexList
				, spaceDivideTree->leafNodeMap
				, pickPoint);

			if (pickNode)
			{
				picking->FindChangeVertex(pickPoint, radius, spaceDivideTree->leafNodeMap.size()
					, pickNode
					, vertices);

				switch (pickingMode)
				{
				case(0):
					UpdateVertexHeight(pickPoint);
					break;
				case(1):
					splatting->TillingTexture(pickPoint, tillingTextureNum, vertices, picking->UpdateVertexIdxList);
					break;
				case(2):
					if(InputManager::GetInstance().GetMouseState(0) == KeyState::PUSH)
						ObjectSpawn(pickPoint);
					break;
				default:
					break;
				}

				// update vertex buffer
				spaceDivideTree->UpdateVertex(picking->UpdateNodeIdxList);
			}
		}

		// save
		if (InputManager::GetInstance().GetKeyState(DIK_M) == KeyState::PUSH)
		{
			heightMap->SaveHeightMap(L"../../Res/Textures/Terrain/height129.PNG");
			splatting->SaveAlphaTexture(L"../../Res/Textures/Terrain/heightExported.PNG");
		}
	}

	spaceDivideTree->Update();
	splatting->SetSRV(shader);

	// temp
	renderMgr->Update();
}

void Terrain::Render()
{
	shader->GetSRV("MapBaseTexture")->SetResource(texture->GetShaderResourceView().Get());

	spaceDivideTree->Render();
}

// -------------------------------------------------------------------------------
// ------------------------------private functions -------------------------------
// -------------------------------------------------------------------------------


void Terrain::ObjectSpawn(Vector3 spawnPos)
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();

	obj->GetTransform()->SetWorldPosition(spawnPos);
	obj->GetTransform()->SetWorldScale(Vector3(0.01f));
	obj->AddComponent(std::make_shared<ModelRenderer>(objectShader));

	obj->GetModelRenderer()->SetModel(model);
	obj->GetModelRenderer()->SetPass(1);

	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
}


void Terrain::UpdateVertexHeight(Vector3 centerPos)
{
	float distance = 0.0f;
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();


	Vector2 center = Vector2(centerPos.x, centerPos.z);
	for (UINT i : picking->UpdateVertexIdxList)
	{
		distance = ( Vector2(vertices[i].position.x, vertices[i].position.z) - center).Length();
		distance = (distance / radius);
		distance = -(distance - 1);

		switch (changeHeightMode)
		{
		case 0:
			vertices[i].position.y += (distance * changeHeight * deltaTime);
			break;
		case 1:
			vertices[i].position.y += (changeHeight * deltaTime);
			break;
		default:
			break;
		}

		heightMap->heightList[i] = vertices[i].position.y;
	}
}

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

			tY = heightMap->GetHeightByIdx(iVertexIndex);
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

void Terrain::CreateLeafNodeIndexList()
{
	UINT leafRowCellNum = rowNum / pow(2, devideTreeDepth);

	leafNodeIndexList.resize(leafRowCellNum * leafRowCellNum * 6);

	UINT colNum = leafRowCellNum + 1;

	UINT iIndex = 0;
	for (int iRow = 0; iRow < leafRowCellNum; ++iRow)
	{
		for (int iCol = 0; iCol < leafRowCellNum; ++iCol)
		{
			int nextCol = iCol + 1;
			int nextRow = iRow + 1;

			leafNodeIndexList[iIndex + 0] = iRow * colNum + iCol;
			leafNodeIndexList[iIndex + 1] = iRow * colNum + nextCol;
			leafNodeIndexList[iIndex + 2] = nextRow * colNum + iCol;

			leafNodeIndexList[iIndex + 3] = leafNodeIndexList[iIndex + 2];
			leafNodeIndexList[iIndex + 4] = leafNodeIndexList[iIndex + 1];
			leafNodeIndexList[iIndex + 5] = nextRow * colNum + nextCol;

			iIndex += 6;
		}
	}
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