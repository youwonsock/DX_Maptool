 #include "pch.h"
#include "NormalMappingDemo.h"

#include "TestCamera.h"

void NormalMappingDemo::Init()
{
	shader = std::make_shared<Shader>(L"14. NormalMapping.fx"); 

	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		{
			material->SetShader(shader);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"LemonChim", L"IMG/Leather.jpg" );
			material->SetDiffuseMap(textue);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"LeatherNormal", L"IMG/Leather_Normal.jpg");
			material->SetNormalMap(textue);
		}
		MaterialDesc& materialDesc = material->GetMaterialDesc();
		materialDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);

		ResourceManager::GetInstance().Add(L"LemonChimMat", material);
	}

	// obj1
	obj1 = std::make_shared<GameObject>();
	obj1->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
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
	obj2->GetTransform()->SetWorldPosition(Vector3(0.5, 0, 2.f));
	obj2->AddComponent(std::make_shared<MeshRenderer>());
	{
		auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Cube");
		obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto material = ResourceManager::GetInstance().Get<Material>(L"LemonChimMat")->Clone();
		obj2->GetMeshRenderer()->SetMaterial(material);
	}


	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform()->SetWorldPosition(Vector3(0,0,-10));
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());

	RenderManager::GetInstance().Init(shader);
}

void NormalMappingDemo::FixedUpdate()
{
}

void NormalMappingDemo::Update()
{
	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void NormalMappingDemo::PostUpdate()
{
}

void NormalMappingDemo::PreRender()
{
}

void NormalMappingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set global light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(0.5f);
		lightDesc.diffuse = Vector4(1.0f);
		lightDesc.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		lightDesc.direction = Vector3(1.0f, 0.f, 1.0f);

		RenderManager::GetInstance().PushLightData(lightDesc);
	}

	// set obj material
	{
		obj1->Update();
	} 
	{
		obj2->Update();
	}
}

void NormalMappingDemo::PostRender()
{
}

void NormalMappingDemo::Release()
{
}
