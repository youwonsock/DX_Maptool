#include "pch.h"
#include "MeshDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"
#include "Engine/ResourceBase.h"

void MeshDemo::Init()
{
	shader = std::make_shared<Shader>(L"07. Normal.fx");

	obj = std::make_shared<GameObject>();
	obj->GetTransform();
	obj->AddComponent(std::make_shared<MeshRenderer>());
	{
		ResourceManager::GetInstance().Init();

		auto shader = std::make_shared<Shader>(L"07. Normal.fx");
		obj->GetMeshRenderer()->SetShader(shader);
	}
	{
		auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
		obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"IMG/Chim.jpg");
		obj->GetMeshRenderer()->SetTexture(texture);
	}

	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform();
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());
}

void MeshDemo::FixedUpdate()
{
}

void MeshDemo::Update()
{
	cameraObject->Update();
}

void MeshDemo::PostUpdate()
{
}

void MeshDemo::PreRender()
{
}

void MeshDemo::Render()
{
	obj->Update();
}

void MeshDemo::PostRender()
{
}

void MeshDemo::Release()
{
}
