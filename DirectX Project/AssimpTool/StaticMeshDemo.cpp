#include "pch.h"
#include "StaticMeshDemo.h"
#include "TestCamera.h"

#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/RenderManager.h"
#include "Engine/GameObject.h"

void StaticMeshDemo::Init()
{
	shader = std::make_shared<Shader>(L"15. ModelDemo.fx");

	camera = std::make_shared<GameObject>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
	camera->AddComponent(std::make_shared<Camera>());
	camera->AddComponent(std::make_shared<TestCamera>());

	CreateTower();

	RenderManager::GetInstance().Init(shader);
}

void StaticMeshDemo::FixedUpdate()
{
}

void StaticMeshDemo::Update()
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

	{
		obj->Update();
	}
}

void StaticMeshDemo::PostUpdate()
{
}

void StaticMeshDemo::PreRender()
{
}

void StaticMeshDemo::Render()
{
}

void StaticMeshDemo::PostRender()
{
}

void StaticMeshDemo::Release()
{
}


void StaticMeshDemo::CreateTower()
{
	// CustomData -> Memory
	std::shared_ptr<class Model> m1 = std::make_shared<Model>();
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");

	obj = std::make_shared<GameObject>();
	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 50));
	obj->GetTransform()->SetWorldScale(Vector3(1.0f));

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	{
		obj->GetModelRenderer()->SetModel(m1);
		//_obj->GetModelRenderer()->SetPass(1);
	}
}

void StaticMeshDemo::CreateTank()
{
	// CustomData -> Memory
	std::shared_ptr<class Model> m1 = std::make_shared<Model>();
	m1->ReadModel(L"Tank/Tank");
	m1->ReadMaterial(L"Tank/Tank");

	obj = std::make_shared<GameObject>();
	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 50));
	obj->GetTransform()->SetWorldScale(Vector3(1.0f));

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	{
		obj->GetModelRenderer()->SetModel(m1);
		//_obj->GetModelRenderer()->SetPass(1);
	}
}
