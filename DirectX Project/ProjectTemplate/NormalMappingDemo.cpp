 #include "pch.h"
#include "NormalMappingDemo.h"

#include "TestCamera.h"

void NormalMappingDemo::Init()
{
	shader = std::make_shared<Shader>(L"14. NormalMapping.fx"); 
	
	{	// old lemon chim
		std::shared_ptr<Material> material = std::make_shared<Material>();
		{
			material->SetShader(shader);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"OldLemonChim", L"IMG/OldGreenLemonChim.png" );
			material->SetDiffuseMap(textue);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"OldLemonNormal", L"IMG/OldGreenLemonChimNormal.png");
			material->SetNormalMap(textue);
		}
		MaterialDesc& materialDesc = material->GetMaterialDesc();
		materialDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.emissive = Vector4(0.5f);

		ResourceManager::GetInstance().Add(L"OldLemonChimMat", material);
	}

	{	// lemon chim
		std::shared_ptr<Material> material = std::make_shared<Material>();
		{
			material->SetShader(shader);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"LemonChim", L"IMG/YellowLemonChim.png");
			material->SetDiffuseMap(textue);
		}
		{
			auto textue = ResourceManager::GetInstance().Load<Texture>(L"LemonNormal", L"IMG/LemonChimNormal.png");
			material->SetNormalMap(textue);
		}
		MaterialDesc& materialDesc = material->GetMaterialDesc();
		materialDesc.ambient = Color(1.f, 1.0f, 1.0f, 1.0f);
		materialDesc.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
		materialDesc.emissive = Vector4(0.5f);

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
		auto material = ResourceManager::GetInstance().Get<Material>(L"OldLemonChimMat");
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
		auto material = ResourceManager::GetInstance().Get<Material>(L"LemonChimMat");
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

Vector3 rotate1 = Vector3(0,0,0);
Vector3 rotate2 = Vector3(0,0,0);
void NormalMappingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set global light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(1.f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(1.f);
		lightDesc.emissive = Vector4(1.0f);
		lightDesc.direction = Vector3(1.0f, 0.f, 1.0f);

		RenderManager::GetInstance().PushLightData(lightDesc);
	}

	// set obj material
	{
		rotate1.y += 0.0001f;
		obj1->GetTransform()->SetWorldRotation(rotate1);
		obj1->Update();
	} 
	{
		rotate2.y += 0.0001f;
		rotate2.z += 0.00005f;
		obj2->GetTransform()->SetWorldRotation(rotate2);
		obj2->Update();
	}
}

void NormalMappingDemo::PostRender()
{
}

void NormalMappingDemo::Release()
{
}
