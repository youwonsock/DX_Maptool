#include "pch.h"
#include "Terrain.h"

#include "SpaceDivideTree.h"
#include "SectionNode.h"

#include "HeightMap.h"
#include "Splatting.h"
#include "MapRenderer.h"
#include "Picking.h"
#include "ObjectManager.h"

#include <fstream>



// todo : desc를 이용한 초기화를 load로 이전 예정
Terrain::Terrain() : Base(ComponentType::Terrain)
{
	// 생성자에서 초기화 할 것들
	{
		heightMap = std::make_shared<HeightMap>();
		splatting = std::make_shared<Splatting>();
		spaceDivideTree = std::make_shared<SpaceDivideTree>();
		picking = std::make_shared<Picking>();
		mapRenderer = std::make_shared<MapRenderer>();

		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
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
	LoadMapData(L"");
}

void Terrain::Update()
{
	{
		if (InputManager::GetInstance().GetMouseState(0) > KeyState::UP)
		{
			auto& ray = CameraManager::GetInstance().GetMainCamera()->GetRay();
			Vector3 pickPoint;

			bool isFind = false;
			std::shared_ptr<SectionNode>& pickNode = picking->FindPickFace(ray, leafNodeIndexList, spaceDivideTree->leafNodeMap, pickPoint);

			if (pickNode)
			{
				switch (mode)
				{
				case(Mode_Height):
					picking->FindChangeVertex(pickPoint, brushSize, spaceDivideTree->leafNodeMap.size(), pickNode, vertices);
					heightMap->UpdateVertexHeight(vertices, picking->UpdateVertexIdxList, pickPoint, brushSize);
					break;
				case(Mode_Splatting):
					picking->FindChangeVertex(pickPoint, brushSize, spaceDivideTree->leafNodeMap.size(), pickNode, vertices);
					splatting->TillingTexture(pickPoint, brushSize, vertices, picking->UpdateVertexIdxList);
					break;
				case(Mode_Object):
					if(InputManager::GetInstance().GetMouseState(0) == KeyState::PUSH)
						spaceDivideTree->SpawnObject(pickPoint);
					break;
				default:
					break;
				}

				// update vertex buffer

				if(mode != Mode_Object && mode != Mode_ObjPicking)
					spaceDivideTree->UpdateVertex(picking->UpdateNodeIdxList);
			}
			switch (mode)
			{
			case(Mode_ObjPicking):
				if (InputManager::GetInstance().GetMouseState(0) == KeyState::PUSH)
					spaceDivideTree->ObjectPicking(ray);
				break;
			default:
				break;
			}
		}
	}

	spaceDivideTree->Update();
	mapRenderer->Update();

	// normal test
	//CalcPerVertexNormalsFastLookup();
}

bool save = false;
bool load = false;
bool create = false;

void Terrain::PostUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				save = true;
				load = false;
				create = false;
				ImGuiFileDialog::Instance()->Close();
			}
			if (ImGui::MenuItem("Load"))
			{
				load = true;
				save = false;
				create = false;
				ImGuiFileDialog::Instance()->Close();
			}
			if (ImGui::MenuItem("Create"))
			{
				create = true;
				save = false;
				load = false;
				ImGuiFileDialog::Instance()->Close();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if(ImGui::Begin("Terrain", nullptr, window_flags))
	{
		// save, load, create
		{
			if (save)
			{
				ImGuiFileDialog::Instance()->OpenDialog("SaveMap", "Save MapData", ".mapData", "../../Res/MapData/");

				if (ImGuiFileDialog::Instance()->Display("SaveMap"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
						std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

						std::wstring fullFileName = Utils::StringToWString(filePath + "\\" + fileName);

						SaveMapData(fullFileName);
					}

					// close
					ImGuiFileDialog::Instance()->Close();

					save = false;
				}
			}
			if (load)
			{
				ImGuiFileDialog::Instance()->OpenDialog("LoadMap", "Choose MapData", ".mapData", "../../Res/MapData/");

				// display
				if (ImGuiFileDialog::Instance()->Display("LoadMap"))
				{
					// action if OK
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
						std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

						std::wstring fullFileName = Utils::StringToWString(filePath + "\\" + fileName);

						LoadMapData(fullFileName);
					}

					// close
					ImGuiFileDialog::Instance()->Close();

					load = false;
				}
			}
			if (create)
			{
				ImGui::OpenPopup("Create");

				if (ImGui::BeginPopup("Create"))
				{
					ImGui::InputFloat("HeightScale", &mapDataDesc.heightScale);
					ImGui::InputFloat("DevideTreeDepth", &mapDataDesc.devideTreeDepth);
					ImGui::InputFloat("CellDistance", &mapDataDesc.cellDistance);
					ImGui::InputInt("Row, Col Num", (int*)&mapDataDesc.rowNum);
					mapDataDesc.colNum = mapDataDesc.rowNum;

					if (ImGui::Button("Create"))
					{
						LoadMapData(L"");

						create = false;
					}
					if (ImGui::Button("Cancel"))
						create = false;

					ImGui::EndPopup();
				}
			}
		}


		ImGui::SetWindowSize("Terrain", ImVec2(350, Global::g_windowHeight));
		ImGui::SetWindowPos("Terrain", ImVec2(Global::g_windowWidth - 350, 0));

		if (ImGui::RadioButton("Object", mode == Mode::Mode_Object))
		{
			mode = Mode::Mode_Object;
		}ImGui::SameLine();
		if (ImGui::RadioButton("Obj Picking", mode == Mode::Mode_ObjPicking))
		{
			mode = Mode::Mode_ObjPicking;
		}ImGui::SameLine();
		if (ImGui::RadioButton("Height", mode == Mode::Mode_Height))
		{
			mode = Mode::Mode_Height;
		}ImGui::SameLine();
		if (ImGui::RadioButton("Texture", mode == Mode::Mode_Splatting))
		{
			mode = Mode::Mode_Splatting;
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (mode == Mode::Mode_Splatting || mode == Mode::Mode_Height)
		{
			ImGui::Text("Set Brush");
			ImGui::SliderFloat("Brush Size", &brushSize, 10, 100);
		}

		switch (mode)
		{
		case Mode_Height:
			heightMap->ShowUI();
			break;
		case Mode_Splatting:
			splatting->ShowUI();
			break;
		case Mode_Object:
			spaceDivideTree->ShowObjectUI();
			break;
		case Mode_ObjPicking:
			spaceDivideTree->ShowObjectPickingUI();
			break;
		default:
			break;
		}

		ImGui::End();
	}
}

void Terrain::Render()
{
	spaceDivideTree->Render();
}

void Terrain::SaveMapData(std::wstring mapDataPath)
{
	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(mapDataPath, FileMode::Write);

	file->Write<std::string>(Utils::WStringToString(baseTexturePath));
	file->Write<std::string>(Utils::WStringToString(splattingDataPath));
	file->Write<std::string>(Utils::WStringToString(heightMapFilePath));
	file->Write<std::string>(Utils::WStringToString(sceneFilePath));

	file->Write<float>(heightScale);
	file->Write<UINT>(rowNum);
	file->Write<UINT>(colNum);
	file->Write<int>(devideTreeDepth);
	file->Write<float>(cellDistance);

	heightMap->SaveHeightMap(heightMapFilePath);

	PathString alphaPath(heightMapFilePath.c_str());
	std::wstring alphaDrive = alphaPath.Drive;
	std::wstring alphaDir = alphaPath.Dir;
	std::wstring alphaFileName = alphaPath.Filename;
	std::wstring alphaTexPath = alphaDrive + alphaDir + alphaFileName + L"_alpha.png";

	splatting->Save(splattingDataPath, alphaTexPath);

	spaceDivideTree->SaveScene(sceneFilePath);
}

void Terrain::LoadMapData(std::wstring mapDataPath)
{
	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();

	heightScale = mapDataDesc.heightScale;
	this->rowNum = mapDataDesc.rowNum;
	this->colNum = mapDataDesc.colNum;
	devideTreeDepth = mapDataDesc.devideTreeDepth;
	cellDistance = mapDataDesc.cellDistance;

	baseTexturePath = L"../../Res/MapData/grass.jpg";
	splattingDataPath = L"../../Res/MapData/splattingData" + std::to_wstring(rowNum) + L".splattingData";
	heightMapFilePath = L"../../Res/MapData/heightMapTexture" + std::to_wstring(rowNum) + L".png";
	sceneFilePath = L"../../Res/MapData/sceneFile" + std::to_wstring(rowNum) + L".sceneData";

	if (file->Open(mapDataPath, FileMode::Read))
	{
		std::string baseTexture = file->Read<std::string>();
		baseTexturePath = Utils::StringToWString(baseTexture);

		std::string splattingData = file->Read<std::string>();
		splattingDataPath = Utils::StringToWString(splattingData);

		std::string heightMap = file->Read<std::string>();
		heightMapFilePath = Utils::StringToWString(heightMap);

		std::string scene = file->Read<std::string>();
		sceneFilePath = Utils::StringToWString(scene);

		heightScale = file->Read<float>();
		rowNum = file->Read<UINT>();
		colNum = file->Read<UINT>();
		devideTreeDepth = file->Read<int>();
		cellDistance = file->Read<float>();
	}

	// set resource, make picking, map renderer
	{
		shader = ResourceManager::GetInstance().Load<Shader>(L"MapToolShader", L"Shader/MapToolShader/MapToolShader.fx");

		texture = ResourceManager::GetInstance().Load<Texture>(L"MapToolTexture", baseTexturePath, false);
		shader->GetSRV("MapBaseTexture")->SetResource(texture->GetShaderResourceView().Get());
	}

	// height map
	{
		heightMap->Init(rowNum, colNum, heightScale, heightMapFilePath);

		// after height map create rowNum, colNum is power of 2 + 1
		rowCellNum = rowNum - 1;
		colCellNum = colNum - 1;
		vertexCount = rowNum * colNum;
		faceCount = rowCellNum * colCellNum * 2;
	}

	// splatting
	{
		splatting->Init(splattingDataPath, rowNum, colNum);
	}

	CreateVertexData();
	CreateIndexData();

	// GenerateVertexNormal, make normal table;
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();

	// quad tree
	spaceDivideTree->Init(shared_from_this());

	// init color
	splatting->SetVertexByTexture(vertices);

	// create leaf node index list(for picking)
	CreateLeafNodeIndexList();

	// init map renderer
	mapRenderer->Init();
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

	//CalcVertexColor(Vector3(0,-1,0));
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