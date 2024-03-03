#include "pch.h"
#include "MapRenderer.h"

#include "SpaceDivideTree.h"
#include "ObjectManager.h"
#include "Engine/ModelRenderer.h"

// map renderer의 skybox와 환경 매핑은 skybox 오브젝트로 이전 하고,
// shadow는 manager를 만들거나 ShadowRenderer를 만들어 이전하자.

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
	cubemapTexture->LoadCubemapTexture(L"../../Res/Textures/Terrain/grassenvmap1024.dds");


	cubemapSRV = skyboxShader->GetSRV("CubeMapTexture");
	HRESULT hr = cubemapSRV->SetResource(cubemapTexture->GetShaderResourceView().Get());

	box = ResourceManager::GetInstance().Get<Mesh>(L"Cube");

	mappingMesh = ResourceManager::GetInstance().Get<Mesh>(L"Cube");

	depthMapShadow.Init();
}

void MapRenderer::Render()
{
	//UINT stride = box->GetVertexBuffer()->GetStride();
	//UINT offset = box->GetVertexBuffer()->GetOffset();

	//Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Global::g_immediateContext->IASetVertexBuffers(0, 1, box->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	//Global::g_immediateContext->IASetIndexBuffer(box->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//skyboxShader->DrawIndexed(0, 0, box->GetIndexBuffer()->GetIndexCount());

	//// 환경 매핑
	//{
	//	transformDesc.World = Matrix::CreateScale(Vector3(0,1,0));

	//	skyboxShader->PushTransformData(transformDesc);

	//	stride = mappingMesh->GetVertexBuffer()->GetStride();
	//	offset = mappingMesh->GetVertexBuffer()->GetOffset();

	//	Global::g_immediateContext->IASetVertexBuffers(0, 1, mappingMesh->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	//	Global::g_immediateContext->IASetIndexBuffer(mappingMesh->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//	skyboxShader->DrawIndexed(0, 1, mappingMesh->GetIndexBuffer()->GetIndexCount());
	//}


}

void MapRenderer::RenderShadow()
{
	depthMapShadow.renderTarget.Begin();

	terrainShader->PushGlobalData(depthMapShadow.lightView, depthMapShadow.lightProj);
	objectShader->PushGlobalData(depthMapShadow.lightView, depthMapShadow.lightProj);

	SceneManager::GetInstance().GetCurrentScene()->Render();

	depthMapShadow.renderTarget.End();

	ShadowDesc desc;
	desc.shadowViewProjection = depthMapShadow.shadowMatrix;
	terrainShader->PushShadowData(desc);
	objectShader->PushShadowData(desc);

	terrainShader->GetSRV("ShadowMap")->SetResource(depthMapShadow.renderTarget.textureSRV.Get());
	objectShader->GetSRV("ShadowMap")->SetResource(depthMapShadow.renderTarget.textureSRV.Get() );

	// set view, proj again (temp)
	view = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	terrainShader->PushGlobalData(view, proj);
	objectShader->PushGlobalData(view, proj);
}

void MapRenderer::Update()
{
	view = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	terrainShader->PushGlobalData(view, proj);
	objectShader->PushGlobalData(view, proj);
	
	skyboxShader->PushGlobalData(view, proj);
	
	transformDesc.World = Matrix::Identity;
	skyboxShader->PushTransformData(transformDesc);

	// set global light
	{
		double gt = TimeManager::GetInstance().GetGameTime();
		//lightDesc.direction = Vector3(cosf(gt), -1.0f, sinf(gt));
		lightDesc.direction = Vector3(1, -1.0f, 1);

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
