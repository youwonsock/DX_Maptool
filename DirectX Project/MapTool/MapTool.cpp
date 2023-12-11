#include "pch.h"
#include "MapTool.h"

#include "Terrain.h"

void MapTool::Init()
{
	{
		TerrainDesc info;
		
		info.rowNum = 100;
		info.colNum = 100;
		info.cellDistance = 1;
		info.heightScale = 0.25f;
		info.shaderFilePath = L"MapToolShader/MapToolShader.fx";
		info.heightMapFilePath = L"../../Res/Textures/Terrain/height129.PNG";
		info.textureFilePath = L"../../Res/Textures/Terrain/grass.jpg";

		// if false, use height map image
		// true : use yasset
		info.useHeightMapByYASSET = false;

		map = std::make_shared<GameObject>();
		map->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		map->AddComponent(std::make_shared<Terrain>(info));

		map->Init();
	}

	// camera
	{
		cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));
	}
}

void MapTool::FixedUpdate()
{
	cameraObject->FixedUpdate();
	map->FixedUpdate();
}

void MapTool::Update()
{
	cameraObject->Update();
	map->Update();
}

void MapTool::PostUpdate()
{
	cameraObject->PostUpdate();
	map->PostUpdate();
}

void MapTool::PreRender()
{
	cameraObject->PreRender();
	map->PreRender();
}

void MapTool::Render()
{
	cameraObject->Render();
	map->Render();
}

void MapTool::PostRender()
{
	cameraObject->PostRender();
	map->PostRender();
}

void MapTool::Release()
{
	cameraObject->Release();
	map->Release();
}
