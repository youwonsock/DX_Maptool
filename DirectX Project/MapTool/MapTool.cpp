#include "pch.h"
#include "MapTool.h"

void MapTool::Init()
{
	shader = std::make_shared<Shader>(L"MapToolShader\\MapToolShader.fx");

	{
		//set height map info
		heightMapComponent = std::make_shared<HeightMap>(shader);

		MapInfo info;

		info.columnNum = 129;
		info.rowNum = 129;
		info.cellDistance = 1.0f;
		info.heightMapTextureFileName = L"../../Res/Textures/Terrain/height2.png";
		info.mapTextureFileName = L"../../Res/Textures/Chim/Chim.jpg";

		heightMapComponent->Set(info);

		map = std::make_shared<GameObject>();
		map->AddComponent(heightMapComponent);

		map->Init();


		map->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));
	}

	// camera
	{
		cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 850, -450));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(45, 0, 0));
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
