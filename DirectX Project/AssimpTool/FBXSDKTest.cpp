#include "pch.h"
#include "FBXSDKTest.h"
#include "Engine/TestCamera.h"

#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/RenderManager.h"
#include "Engine/GameObject.h"

void FBXSDKTest::CreateKachujin()
{
}

void FBXSDKTest::Init()
{
	shader = std::make_shared<Shader>(L"MapToolShader/ObjectShader.fx");
	
	debugDrawer = std::make_shared<DebugDrawer>();

	//camera
	{
		std::shared_ptr<GameObject>cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
	}


	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->ReadModel(L"Tower/Tower");
	model->ReadMaterial(L"Tower/Tower");

	obj1 = std::make_shared<GameObject>();

	obj1->AddComponent(std::make_shared<ModelRenderer>(shader));
	obj1->GetModelRenderer()->SetModel(model);
	obj1->GetModelRenderer()->SetPass(0);

	obj1->GetTransform()->SetWorldPosition(Vector3(5, 5, 0));
	obj1->GetTransform()->SetWorldScale(Vector3(0.5f,0.5f,0.5f));
	//obj1->GetTransform()->SetWorldScale(Vector3(1.f,1.f,1.f));
	obj1->GetTransform()->SetWorldRotation(Vector3(90, 0, 90));

	SceneManager::GetInstance().GetCurrentScene()->Add(obj1);

	//instancing
	{
		int count = 300;
		// tower
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			model->ReadModel(L"Tower/Tower");
			model->ReadMaterial(L"Tower/Tower");

			for (int i = 0; i < count; ++i)
			{
				std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
				obj->GetTransform()->SetWorldPosition(Vector3(rand() % 1000, 0, rand() % 1000));
				obj->GetTransform()->SetWorldScale(Vector3(3.f));
				obj->GetTransform()->SetWorldRotation(Vector3(rand() % 360, rand() % 360, rand() % 360));

				obj->AddComponent(std::make_shared<ModelRenderer>(shader));
				obj->GetModelRenderer()->SetModel(model);
				obj->GetModelRenderer()->SetPass(0);

				SceneManager::GetInstance().GetCurrentScene()->Add(obj);
			}
		}
		// ship
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			model->ReadModel(L"ship/ship");
			model->ReadMaterial(L"ship/ship");

			for (int i = 0; i < count; ++i)
			{
				std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
				obj->GetTransform()->SetWorldPosition(Vector3(rand() % 1000, 0, rand() % 1000));
				obj->GetTransform()->SetWorldScale(Vector3(0.1f));
				obj->GetTransform()->SetWorldRotation(Vector3(rand() % 360, rand() % 360, rand() % 360));

				obj->AddComponent(std::make_shared<ModelRenderer>(shader));
				obj->GetModelRenderer()->SetModel(model);
				obj->GetModelRenderer()->SetPass(0);

				SceneManager::GetInstance().GetCurrentScene()->Add(obj);
			}
		}
		// multi
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			model->ReadModel(L"MultiCameras/MultiCameras");
			model->ReadMaterial(L"MultiCameras/MultiCameras");

			for (int i = 0; i < count; ++i)
			{
				std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
				obj->GetTransform()->SetWorldPosition(Vector3(rand() % 1000, 0, rand() % 1000));
				obj->GetTransform()->SetWorldScale(Vector3(0.1f));
				obj->GetTransform()->SetWorldRotation(Vector3(rand() % 360, rand() % 360, rand() % 360));

				obj->AddComponent(std::make_shared<ModelRenderer>(shader));
				obj->GetModelRenderer()->SetModel(model);
				obj->GetModelRenderer()->SetPass(0);

				SceneManager::GetInstance().GetCurrentScene()->Add(obj);
			}
		}
		// turret
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			model->ReadModel(L"Turret_Deploy1/Turret_Deploy1");
			model->ReadMaterial(L"Turret_Deploy1/Turret_Deploy1");
			model->ReadAnimation(L"Turret_Deploy1/Turret_Deploy1");

			for (int i = 0; i < count; ++i)
			{
				std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
				obj->GetTransform()->SetWorldPosition(Vector3(rand() % 1000, 0, rand() % 1000));
				obj->GetTransform()->SetWorldScale(Vector3(1.f));
				obj->GetTransform()->SetWorldRotation(Vector3(rand() % 360, rand() % 360, rand() % 360));

				obj->AddComponent(std::make_shared<ModelRenderer>(shader));
				obj->GetModelRenderer()->SetModel(model);
				obj->GetModelRenderer()->SetPass(1);

				SceneManager::GetInstance().GetCurrentScene()->Add(obj);
			}
		}
	}

	debugDrawer->DrawBox(obj1->GetTransform()->GetBoundingBox(), Color(1,0,0,0));

	RenderManager::GetInstance().Init(shader);
}

void FBXSDKTest::FixedUpdate()
{
}

void FBXSDKTest::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(0.4f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(0.f);
		lightDesc.direction = Vector3(1.f, 0.f, 1.f);
		RenderManager::GetInstance().PushLightData(lightDesc);
	}
	obj1->Update();

	debugDrawer->Update();
}

void FBXSDKTest::PostUpdate()
{
}

void FBXSDKTest::PreRender()
{
}

void FBXSDKTest::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj1->Render();

	Matrix t = obj1->GetTransform()->GetWorldMatrix();
	debugDrawer->Render(&t);
}

void FBXSDKTest::PostRender()
{
}

void FBXSDKTest::Release()
{
}
