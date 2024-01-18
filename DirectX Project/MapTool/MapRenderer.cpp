#include "pch.h"
#include "MapRenderer.h"

void MapRenderer::Init()
{
	terrainShader = ResourceManager::GetInstance().Get<Shader>(L"MapToolShader");
	objectShader = ResourceManager::GetInstance().Get<Shader>(L"ObjectShader");

	transformDesc.World = Matrix::Identity;
	terrainShader->PushTransformData(transformDesc);

	lightDesc.ambient = Vector4(1.f);
	lightDesc.diffuse = Vector4(1.f);
	lightDesc.specular = Vector4(1.f);
	lightDesc.emissive = Vector4(1.0f);


	skyboxShader = ResourceManager::GetInstance().Load<Shader>(L"SkyboxShader", L"Shader/MapToolShader/SkyBox.fx");
	cubemapTexture->Load(L"../../Res/Textures/Terrain/grassenvmap1024.dds");
}

void MapRenderer::Update()
{z
	view = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	terrainShader->PushGlobalData(view, proj);
	objectShader->PushGlobalData(view, proj);
	
	skyboxShader->PushGlobalData(view, proj);
	
	transformDesc.World = CameraManager::GetInstance().GetMainCamera()->GetTransform()->GetWorldMatrix();
	skyboxShader->PushTransformData(transformDesc);

	// set global light
	{
		double gt = TimeManager::GetInstance().GetGameTime();
		lightDesc.direction = Vector3(cosf(gt), -1.0f, sinf(gt));

		objectShader->PushLightData(lightDesc);
		terrainShader->PushLightData(lightDesc);
	}
}

MapRenderer::MapRenderer()
{
}

MapRenderer::~MapRenderer()
{
}
