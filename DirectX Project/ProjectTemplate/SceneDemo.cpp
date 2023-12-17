 #include "pch.h"
#include "SceneDemo.h"

#include "Engine/TestCamera.h"

void SceneDemo::Init()
{
	shader = std::make_shared<Shader>(L"23. RenderDemo.fx");

	// camera
	{	
		auto cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		SceneManager::GetInstance().GetCurrentScene()->Add(cameraObject);
	}

	// light
	{
		auto light = std::make_shared<GameObject>();
		light->AddComponent(std::make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(0.4f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(0.1f);
		lightDesc.direction = Vector3(1.f, 0.f, 1.f);
		light->GetLight()->SetLightDesc(lightDesc); 
		SceneManager::GetInstance().GetCurrentScene()->Add(light);
	}
	

	// anim
	std::shared_ptr<Model> model1 = std::make_shared<Model>();
	model1->ReadModel(L"Kachujin/Kachujin");
	model1->ReadMaterial(L"Kachujin/Kachujin");
	model1->ReadAnimation(L"Kachujin/Idle");
	model1->ReadAnimation(L"Kachujin/Run");
	model1->ReadAnimation(L"Kachujin/Slash");

	for (int i = 0; i < 500; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->GetTransform()->SetWorldScale(Vector3(0.01f));

		obj->AddComponent(std::make_shared<ModelAnimator>(shader));

		{
			obj->GetModelAnimator()->SetModel(model1);
			obj->GetModelAnimator()->SetPass(2);
		}

		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	}

	// model
	std::shared_ptr<Model> model2 = std::make_shared<Model>();
	model2->ReadModel(L"Tower/Tower");
	model2->ReadMaterial(L"Tower/Tower");

	for (int i = 0; i < 500; i++)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->GetTransform()->SetWorldScale(Vector3(0.01f));
		obj->AddComponent(std::make_shared<ModelRenderer>(shader));

		obj->GetModelRenderer()->SetModel(model2);
		obj->GetModelRenderer()->SetPass(1);

		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	}

	// mesh

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

	for (int i = 0; i < 500; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->AddComponent(std::make_shared<MeshRenderer>());

		{
			obj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInstance().Get<Material>(L"Chim"));

			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
		}

		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	}


	RenderManager::GetInstance().Init(shader);
}
 
void SceneDemo::FixedUpdate()
{
}

void SceneDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}
}

void SceneDemo::PostUpdate()
{
}

void SceneDemo::PreRender()
{
}

void SceneDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SceneDemo::PostRender()
{
}

void SceneDemo::Release()
{
}
