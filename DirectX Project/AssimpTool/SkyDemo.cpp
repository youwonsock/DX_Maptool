#include "pch.h"
#include "SkyDemo.h"
#include "Engine/TestCamera.h"

#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/RenderManager.h"
#include "Engine/GameObject.h"


void SkyDemo::Init()
{
	shader = std::make_shared<Shader>(L"18. SkyDemo.fx");

	// mat
	{
		std::shared_ptr<Material> mat = std::make_shared<Material>();
		mat->SetShader(shader);
		auto tex = ResourceManager::GetInstance().Load<Texture>(L"Sky", L"Textures/Sky01.jpg");
		mat->SetDiffuseMap(tex);
		MaterialDesc& desc = mat->GetMaterialDesc();
		desc.ambient = Vector4(1.f);
		desc.diffuse = Vector4(1.f);
		desc.specular = Vector4(1.f);
		ResourceManager::GetInstance().Add(L"Sky", mat);
	}

	// sky
	{
		obj = std::make_shared<GameObject>();
		obj->GetTransform();
		obj->AddComponent(std::make_shared<MeshRenderer>());
		{
			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			auto mat = ResourceManager::GetInstance().Get<Material>(L"Sky");
			obj->GetMeshRenderer()->SetMaterial(mat);
		}
	}

	camera = std::make_shared<GameObject>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 0, -5));
	camera->AddComponent(std::make_shared<Camera>());
	camera->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(shader);
}

void SkyDemo::FixedUpdate()
{
}

void SkyDemo::Update()
{
	camera->Update();
	RenderManager::GetInstance().Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(0.4f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(0.f);
		lightDesc.direction = Vector3(1.f, 0.f, 1.f);
		RenderManager::GetInstance().PushLightData(lightDesc);
	}
}

void SkyDemo::PostUpdate()
{
}

void SkyDemo::PreRender()
{
}

void SkyDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj->Update();
}

void SkyDemo::PostRender()
{
}

void SkyDemo::Release()
{
}
