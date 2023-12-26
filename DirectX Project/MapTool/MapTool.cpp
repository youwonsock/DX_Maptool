#include "pch.h"
#include "MapTool.h"

#include "Engine/TestCamera.h"

#include "Terrain.h"
#include "SkyBox.h"

void MapTool::Init()
{
	{
		TerrainDesc info;
		
		info.rowNum = 200;
		info.colNum = 200;
		info.cellDistance = 1;
		info.heightScale = 0.25f;
		info.shaderFilePath = L"MapToolShader/MapToolShader.fx";
		info.heightMapFilePath = L"../../Res/Textures/Terrain/height129.PNG";
		info.alphaTexPath = L"../../Res/Textures/Terrain/heightExported.PNG";
		info.textureFilePath = L"../../Res/Textures/Terrain/grass.jpg";
		info.DevideTreeDepth = 2;

		map = std::make_shared<GameObject>();
		map->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		map->AddComponent(std::make_shared<Terrain>(info));
	}

	// skybox
	{
		//map->AddComponent(std::make_shared<SkyBox>());
	}
		
	map->Init();

	// camera
	{
		cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
	}
}

void MapTool::FixedUpdate()
{
	map->FixedUpdate();
}

void MapTool::Update()
{
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	map->Update();
}

void MapTool::PostUpdate()
{
	map->PostUpdate();

}

void MapTool::PreRender()
{
	map->PreRender();
}

void MapTool::Render()
{
	map->Render();
}

void MapTool::PostRender()
{
	map->PostRender();
}

void MapTool::Release()
{
	map->Release(); 
}