#include "pch.h"
#include "MapTool.h"

void MapTool::Init()
{
	shader = std::make_shared<Shader>(L"MapToolShader\\MapToolShader.fx");

	{
		//set height map info
		heightMapComponent = std::make_shared<HeightMap>();

		map = std::make_shared<GameObject>();
		map->AddComponent(heightMapComponent);
	}

	// camera
	{
		cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(5, 10, -10));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(-10, 0, 0));
	}
}

void MapTool::FixedUpdate()
{
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
}

void MapTool::Render()
{
}

void MapTool::PostRender()
{
}

void MapTool::Release()
{
}
