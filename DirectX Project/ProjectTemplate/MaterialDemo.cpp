#include "pch.h"
#include "MaterialDemo.h"

#include "TestCamera.h"

void MaterialDemo::Init()
{
	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform()->SetWorldPosition(Vector3(1,0,0));
	obj1->AddComponent(std::make_shared<MeshRenderer>());

	shader = std::make_shared<Shader>(L"13. Lighting.fx"); 

	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		{
			material->SetShader(shader);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"IMG/OldGreenLemonChim.png" );
			material->SetDiffuseMap(textue);
		}
		MaterialDesc& materialDesc = material->GetMaterialDesc();
		materialDesc

	}

	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform()->SetWorldPosition(Vector3(0,0,-10));
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(obj1->GetMeshRenderer()->GetShader());
}

void MaterialDemo::FixedUpdate()
{
}

void MaterialDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void MaterialDemo::PostUpdate()
{
}

void MaterialDemo::PreRender()
{
}

void MaterialDemo::Render()
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

void MaterialDemo::PostRender()
{
}

void MaterialDemo::Release()
{
}
