 #include "pch.h"
#include "RenderDemo.h"

#include "Engine/TestCamera.h"

void RenderDemo::Init()
{
	// camera
	{	
		cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
	}
	
	shader = std::make_shared<Shader>(L"23. RenderDemo.fx");

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
		
		objs.push_back(obj);
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

		objs.push_back(obj);
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

		objs.push_back(obj);
	}


	RenderManager::GetInstance().Init(shader);
}
 
void RenderDemo::FixedUpdate()
{
}

void RenderDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void RenderDemo::PostUpdate()
{
}

void RenderDemo::PreRender()
{
}

void RenderDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	InstancingManager::GetInstance().Render(objs);
}

void RenderDemo::PostRender()
{
}

void RenderDemo::Release()
{
}
