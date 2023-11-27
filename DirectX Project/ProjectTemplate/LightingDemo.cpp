#include "pch.h"
#include "LightingDemo.h"

#include "TestCamera.h"

void LightingDemo::Init()
{
	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform()->SetWorldPosition(Vector3(1,0,0));
	obj1->AddComponent(std::make_shared<MeshRenderer>());

	shader = std::make_shared<Shader>(L"13. Lighting.fx");
	obj1->GetMeshRenderer()->SetShader(shader);
	{
		auto mesh1 = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
		obj1->GetMeshRenderer()->SetMesh(mesh1);
	}
	{
		auto texture1 = ResourceManager::GetInstance().Load<Texture>(L"LemonChim", L"IMG/OldGreenLemonChim.png");
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
		auto texture2 = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"IMG/Chim.jpg");
		obj2->GetMeshRenderer()->SetTexture(texture2);
	}


	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform()->SetWorldPosition(Vector3(0,0,-10));
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(obj1->GetMeshRenderer()->GetShader());
}

void LightingDemo::FixedUpdate()
{
}

void LightingDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void LightingDemo::PostUpdate()
{
}

void LightingDemo::PreRender()
{
}

void LightingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set global light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		lightDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		lightDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
		lightDesc.direction = Vector3(1.0f, -1.f, 0.0f);

		RenderManager::GetInstance().PushLightData(lightDesc);
	}

	// set obj material
	{
		MaterialDesc materialDesc;
		materialDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);

		RenderManager::GetInstance().PushMaterialData(materialDesc);
		obj1->Update();
	} 
	{
		MaterialDesc materialDesc;
		materialDesc.ambient = Color(0.5f, 0.5f, 0.5f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		//materialDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);

		RenderManager::GetInstance().PushMaterialData(materialDesc);
		obj2->Update();
	}
}

void LightingDemo::PostRender()
{
}

void LightingDemo::Release()
{
}
