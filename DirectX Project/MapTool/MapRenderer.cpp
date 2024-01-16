#include "pch.h"
#include "MapRenderer.h"

void MapRenderer::Update()
{
	view = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	terrainShader->PushGlobalData(view, proj);
	objectShader->PushGlobalData(view, proj);
}

void MapRenderer::Init()
{
	terrainShader = ResourceManager::GetInstance().Get<Shader>(L"MapToolShader");
	objectShader = ResourceManager::GetInstance().Get<Shader>(L"ObjectShader");

	TransformDesc transformDesc;
	transformDesc.World = Matrix::Identity;
	
	terrainShader->PushTransformData(transformDesc);
}

MapRenderer::MapRenderer()
{
}

MapRenderer::~MapRenderer()
{
}
