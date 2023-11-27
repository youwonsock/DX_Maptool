 #include "pch.h"
#include "MaterialDemo.h"

#include "TestCamera.h"

void MaterialDemo::Init()
{
	shader = std::make_shared<Shader>(L"13. Lighting.fx"); 

	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		{
			material->SetShader(shader);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"LemonChim", L"IMG/OldGreenLemonChim.png" );
			material->SetDiffuseMap(textue);
		}
		MaterialDesc& materialDesc = material->GetMaterialDesc();
		materialDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

		ResourceManager::GetInstance().Add(L"LemonChimMat", material);
	}

	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform()->SetWorldPosition(Vector3(1, 0, 0));
	obj1->AddComponent(std::make_shared<MeshRenderer>());
	{
		auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
		obj1->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto material = ResourceManager::GetInstance().Get<Material>(L"LemonChimMat");
		obj1->GetMeshRenderer()->SetMaterial(material);
	}

	//obj2
	obj2 = std::make_shared<GameObject>();
	obj2->GetTransform()->SetWorldPosition(Vector3(-1, 0, 0));
	obj2->AddComponent(std::make_shared<MeshRenderer>());
	{
		auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Cube");
		obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto material = ResourceManager::GetInstance().Get<Material>(L"LemonChimMat")->Clone();
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Color(1.f, 0.f, 0.f, 1.0f);
		desc.diffuse = Color(0.2f, 0.2f, 0.2f, 0.2f);

		obj2->GetMeshRenderer()->SetMaterial(material);
	}


	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform()->SetWorldPosition(Vector3(0,0,-10));
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(shader);
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
