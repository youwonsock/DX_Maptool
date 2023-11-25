#include "pch.h"
#include "GlobalTestDemo.h"

#include "TestCamera.h"

void GlobalTestDemo::Init()
{
	obj = std::make_shared<GameObject>();
	obj->GetTransform();
	obj->AddComponent(std::make_shared<MeshRenderer>());
	{
		auto shader = std::make_shared<Shader>(L"08. GlobalTest.fx");
		obj->GetMeshRenderer()->SetShader(shader);
	}
	{
		ResourceManager::GetInstance().Init();
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

	RenderManager::GetInstance().Init(obj->GetMeshRenderer()->GetShader());
}

void GlobalTestDemo::FixedUpdate()
{
}

void GlobalTestDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void GlobalTestDemo::PostUpdate()
{
}

void GlobalTestDemo::PreRender()
{
}

void GlobalTestDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj->Update();
}

void GlobalTestDemo::PostRender()
{
}

void GlobalTestDemo::Release()
{
}
