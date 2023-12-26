 #include "pch.h"
#include "SceneDemo.h"

#include "Engine/TestCamera.h"

void SceneDemo::Init()
{
	shader = std::make_shared<Shader>(L"23. RenderDemo.fx");

	// camera
	{
		std::shared_ptr<GameObject>cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
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
		//SceneManager::GetInstance().GetCurrentScene()->Add(light);
	}

	// turret
	//std::shared_ptr<Model> model1 = std::make_shared<Model>();
	//model1->ReadModel(L"Turret_Deploy1/Turret_Deploy1");
	//model1->ReadMaterial(L"Turret_Deploy1/Turret_Deploy1");
	//model1->ReadAnimation(L"Turret_Deploy1/Turret_Deploy1");

	//for (int i = 0; i < 1; ++i)
	//{
	//	auto obj = std::make_shared<GameObject>();
	//	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
	//	//obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
	//	obj->GetTransform()->SetWorldScale(Vector3(1.f));

	//	obj->AddComponent(std::make_shared<ModelAnimator>(shader));

	//	{
	//		obj->GetModelAnimator()->SetModel(model1);
	//		obj->GetModelAnimator()->SetPass(2);
	//	}

	//	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//}	
	
	// ship
	//std::shared_ptr<Model> model1 = std::make_shared<Model>();
	//model1->ReadModel(L"Turret_Deploy1/Turret_Deploy1");
	//model1->ReadMaterial(L"Turret_Deploy1/Turret_Deploy1");
	//model1->ReadAnimation(L"Turret_Deploy1/Turret_Deploy1");

	//for (int i = 0; i < 1; ++i)
	//{
	//	auto obj = std::make_shared<GameObject>();
	//	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
	//	//obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
	//	obj->GetTransform()->SetWorldScale(Vector3(1.f));

	//	obj->AddComponent(std::make_shared<ModelAnimator>(shader));

	//	{
	//		obj->GetModelAnimator()->SetModel(model1);
	//		obj->GetModelAnimator()->SetPass(2);
	//	}

	//	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//}	
	
	// multi camera
	std::shared_ptr<Model> model3 = std::make_shared<Model>();
	model3->ReadModel(L"MultiCameras/MultiCameras");
	model3->ReadMaterial(L"MultiCameras/MultiCameras");

	for (int i = 0; i < 1; ++i)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		obj->GetTransform()->SetWorldScale(Vector3(1.f));
		obj->AddComponent(std::make_shared<ModelRenderer>(shader));

		obj->GetModelRenderer()->SetModel(model3);
		obj->GetModelRenderer()->SetPass(1);

		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	}




	// ---------------------°­ÀÇ-----------------------------

	//// anim
	//std::shared_ptr<Model> model0 = std::make_shared<Model>();
	//model0->ReadModel(L"Kachujin/Kachujin");
	//model0->ReadMaterial(L"Kachujin/Kachujin");
	//model0->ReadAnimation(L"Kachujin/Idle");
	//model0->ReadAnimation(L"Kachujin/Run");
	//model0->ReadAnimation(L"Kachujin/Slash");

	//for (int i = 0; i < 500; ++i)
	//{
	//	auto obj = std::make_shared<GameObject>();
	//	obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
	//	obj->GetTransform()->SetWorldScale(Vector3(0.01f));

	//	obj->AddComponent(std::make_shared<ModelAnimator>(shader));

	//	{
	//		obj->GetModelAnimator()->SetModel(model0);
	//		obj->GetModelAnimator()->SetPass(2);
	//	}

	//	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//}

	//// model
	//std::shared_ptr<Model> model2 = std::make_shared<Model>();
	//model2->ReadModel(L"Tower/Tower");
	//model2->ReadMaterial(L"Tower/Tower");

	//for (int i = 0; i < 500; i++)
	//{
	//	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	//	obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
	//	obj->GetTransform()->SetWorldScale(Vector3(0.01f));
	//	obj->AddComponent(std::make_shared<ModelRenderer>(shader));

	//	obj->GetModelRenderer()->SetModel(model2);
	//	obj->GetModelRenderer()->SetPass(1);

	//	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//}

	//// mesh

	//// Material
	//{
	//	std::shared_ptr<Material> material = std::make_shared<Material>();
	//	material->SetShader(shader);
	//	auto texture = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"../../Res/Textures/Chim/YellowLemonChim.png");
	//	material->SetDiffuseMap(texture); 

	//	MaterialDesc& desc = material->GetMaterialDesc();
	//	desc.ambient = Vector4(1.f);
	//	desc.diffuse = Vector4(1.f);
	//	desc.specular = Vector4(1.f);

	//	ResourceManager::GetInstance().Add(L"Chim", material);
	//}

	//for (int i = 0; i < 500; ++i)
	//{
	//	auto obj = std::make_shared<GameObject>();
	//	obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
	//	obj->AddComponent(std::make_shared<MeshRenderer>());

	//	{
	//		obj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInstance().Get<Material>(L"Chim"));

	//		auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
	//		obj->GetMeshRenderer()->SetMesh(mesh);
	//	}

	//	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//}


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
