#include "pch.h"
#include "MapRenderer.h"

void MapRenderer::Update()
{
	view = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	terrainShader->PushGlobalData(view, proj);
	objectShader->PushGlobalData(view, proj);


	// set global light
	{
		double gt = TimeManager::GetInstance().GetGameTime();
		//lightDesc.direction = Vector3(cosf(gt), -1.0f, sinf(gt));

		objectShader->PushLightData(lightDesc);
		terrainShader->PushLightData(lightDesc);
	}
}

void MapRenderer::Init()
{
	terrainShader = ResourceManager::GetInstance().Get<Shader>(L"MapToolShader");
	objectShader = ResourceManager::GetInstance().Get<Shader>(L"ObjectShader");

	TransformDesc transformDesc;
	transformDesc.World = Matrix::Identity;
	
	terrainShader->PushTransformData(transformDesc);

	lightDesc.ambient = Vector4(1.f);
	lightDesc.diffuse = Vector4(1.f);
	lightDesc.specular = Vector4(1.f);
	lightDesc.emissive = Vector4(1.0f);
	//lightDesc.direction = Vector3(1.0f, -1.0f, 1.0f);
	lightDesc.direction = Vector3(1.0f, 0, 0);
}

MapRenderer::MapRenderer()
{
}

MapRenderer::~MapRenderer()
{
}
