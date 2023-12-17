 #include "pch.h"
#include "AnimInstancingDemo.h"

#include "Engine/TestCamera.h"

void AnimInstancingDemo::Init()
{
	// camera
	{	
		cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
	}
	
	shader = std::make_shared<Shader>(L"22. AnimInstancingDemo.fx");

	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->ReadModel(L"Kachujin/Kachujin");
	model->ReadMaterial(L"Kachujin/Kachujin");
	model->ReadAnimation(L"Kachujin/Idle");
	model->ReadAnimation(L"Kachujin/Run");
	model->ReadAnimation(L"Kachujin/Slash");

	for (int i = 0; i < 500; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->GetTransform()->SetWorldScale(Vector3(0.01f));

		obj->AddComponent(std::make_shared<ModelAnimator>(shader));

		{
			obj->GetModelAnimator()->SetModel(model);
		}
		
		objs.push_back(obj);
	}

	RenderManager::GetInstance().Init(shader);
}
 
void AnimInstancingDemo::FixedUpdate()
{
}

void AnimInstancingDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void AnimInstancingDemo::PostUpdate()
{
}

void AnimInstancingDemo::PreRender()
{
}

void AnimInstancingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	InstancingManager::GetInstance().Render(objs);
}

void AnimInstancingDemo::PostRender()
{
}

void AnimInstancingDemo::Release()
{
}
