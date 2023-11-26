#include "pch.h"
#include "DepthStencilDemo.h"

#include "TestCamera.h"

void DepthStencilDemo::Init()
{
	// engine에서 하면 안되는 이유는?
	ResourceManager::GetInstance().Init();

	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform();
	obj1->AddComponent(std::make_shared<MeshRenderer>());

	auto shader1 = std::make_shared<Shader>(L"08. GlobalTest.fx");
	obj1->GetMeshRenderer()->SetShader(shader1);
	{
		auto mesh1 = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
		obj1->GetMeshRenderer()->SetMesh(mesh1);
	}
	{
		auto texture1 = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"IMG/Chim.jpg");
		obj1->GetMeshRenderer()->SetTexture(texture1);
	}
	// obj2
	obj2 = std::make_shared<GameObject>();
	obj2->GetTransform()->SetWorldPosition(Vector3(0.5f, 0, 2));
	obj2->AddComponent(std::make_shared<MeshRenderer>());
	obj2->GetMeshRenderer()->SetShader(shader1);
	{
		auto mesh2 = ResourceManager::GetInstance().Get<Mesh>(L"Cube");
		obj2->GetMeshRenderer()->SetMesh(mesh2);
	}
	{
		auto texture2 = ResourceManager::GetInstance().Load<Texture>(L"UJAE", L"IMG/UJAE.png");
		obj2->GetMeshRenderer()->SetTexture(texture2);
	}


	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform()->SetWorldPosition(Vector3(0,0,-10));
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(obj1->GetMeshRenderer()->GetShader());
}

void DepthStencilDemo::FixedUpdate()
{
}

void DepthStencilDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void DepthStencilDemo::PostUpdate()
{
}

void DepthStencilDemo::PreRender()
{
}

void DepthStencilDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj1->Update();
	obj2->Update();
}

void DepthStencilDemo::PostRender()
{
}

void DepthStencilDemo::Release()
{
}
