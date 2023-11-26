#include "pch.h"
#include "DiffuseDemo.h"

#include "TestCamera.h"

void DiffuseDemo::Init()
{
	// engine에서 하면 안되는 이유는?
	ResourceManager::GetInstance().Init();

	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform();
	obj1->AddComponent(std::make_shared<MeshRenderer>());

	shader = std::make_shared<Shader>(L"10. Lighting_Diffuse.fx");
	obj1->GetMeshRenderer()->SetShader(shader);
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
	obj2->GetMeshRenderer()->SetShader(shader);
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

void DiffuseDemo::FixedUpdate()
{
}

void DiffuseDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void DiffuseDemo::PostUpdate()
{
}

void DiffuseDemo::PreRender()
{
}

void DiffuseDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set diffuse light
	Vector4 lightDiffuse = { 1.f, 1.f, 1.f, 1.0f };
	shader->GetVector("LightDiffuse")->SetFloatVector((float*)&lightDiffuse);
	
	// set light direction
	Vector3 lightDirection = { 1.f, 0.f, 0.f };
	lightDirection.Normalize();
	shader->GetVector("LightDir")->SetFloatVector((float*)&lightDirection);

	// set obj material
	{
		Vector4 material(1.f);
		shader->GetVector("MatrealDiffuse")->SetFloatVector((float*)&material);
		obj1->Update();
	}
	{
		Vector4 material(0.5f);
		shader->GetVector("MatrealDiffuse")->SetFloatVector((float*)&material);
		obj2->Update();
	}
}

void DiffuseDemo::PostRender()
{
}

void DiffuseDemo::Release()
{
}
