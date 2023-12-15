#include "pch.h"
#include "StaticMeshDemo.h"
#include "Engine/TestCamera.h"

#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/RenderManager.h"
#include "Engine/GameObject.h"

void StaticMeshDemo::Init()
{
	shader = std::make_shared<Shader>(L"15. ModelDemo.fx");

	camera = std::make_shared<GameObject>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 0, -5));
	camera->AddComponent(std::make_shared<Camera>());
	camera->AddComponent(std::make_shared<TestCamera>());

	CreateTower();
	//CreateTank();
	//CreateHouse();

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
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj->Update();
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
	obj->GetTransform()->SetWorldScale(Vector3(0.1f));

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	{
		obj->GetModelRenderer()->SetModel(m1);
		//obj->GetModelRenderer()->SetPass(1);
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
	obj->GetTransform()->SetWorldScale(Vector3(1.f));

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	{
		obj->GetModelRenderer()->SetModel(m1);
		obj->GetModelRenderer()->SetPass(1);
	}
}

void StaticMeshDemo::CreateHouse()
{
	// CustomData -> Memory
	std::shared_ptr<class Model> m1 = std::make_shared<Model>();
	m1->ReadModel(L"House/House");
	m1->ReadMaterial(L"House/House");

	obj = std::make_shared<GameObject>();
	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 10000));
	obj->GetTransform()->SetWorldScale(Vector3(0.5f));

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	{
		obj->GetModelRenderer()->SetModel(m1);
		obj->GetModelRenderer()->SetPass(1);
	}
}
