 #include "pch.h"
#include "MeshInstancingDemo.h"

#include "Engine/TestCamera.h"

void MeshInstancingDemo::Init()
{
	// camera
	{	
		cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
	}
	
	shader = std::make_shared<Shader>(L"20. MeshInstancingDemo.fx");

	// Material
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		auto texture = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"../../Res/Textures/Chim/YellowLemonChim.png");
		material->SetDiffuseMap(texture); 

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vector4(1.f);
		desc.diffuse = Vector4(1.f);
		desc.specular = Vector4(1.f);

		ResourceManager::GetInstance().Add(L"Chim", material);
	}

	for (int i = 0; i < 100000; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->AddComponent(std::make_shared<MeshRenderer>());

		{
			obj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInstance().Get<Material>(L"Chim"));

			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
		}
		
		objs.push_back(obj);
	}

	RenderManager::GetInstance().Init(shader);
}
 
void MeshInstancingDemo::FixedUpdate()
{
}

void MeshInstancingDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void MeshInstancingDemo::PostUpdate()
{
}

void MeshInstancingDemo::PreRender()
{
}

void MeshInstancingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	InstancingManager::GetInstance().Render(objs);
}

void MeshInstancingDemo::PostRender()
{
}

void MeshInstancingDemo::Release()
{
}
