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
	cubemapTexture = std::make_shared<Texture>();
	cubemapTexture->CreateCubemapTexture(L"../../Res/Textures/Terrain/grassenvmap1024.dds");


	cubemapSRV = skyboxShader->GetSRV("CubeMapTexture");
	HRESULT hr = cubemapSRV->SetResource(cubemapTexture->GetShaderResourceView().Get());

	boxs = ResourceManager::GetInstance().Get<Mesh>(L"Cube");
}

void MapRenderer::Render()
{


	UINT stride = box->GetVertexBuffer()->GetStride();
	UINT offset = box->GetVertexBuffer()->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, box->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(box->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	skyboxShader->DrawIndexed(0, 0, box->GetIndexBuffer()->GetIndexCount());
}

void MapRenderer::Update()
{
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
