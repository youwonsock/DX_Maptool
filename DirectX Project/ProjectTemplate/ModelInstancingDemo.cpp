 #include "pch.h"
#include "ModelInstancingDemo.h"

#include "Engine/TestCamera.h"

void ModelInstancingDemo::Init()
{
	// camera
	{	
		cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
	}
	
	shader = std::make_shared<Shader>(L"21. ModelInstancingDemo.fx");

	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->ReadModel(L"Tower/Tower");
	model->ReadMaterial(L"Tower/Tower"); 

	for(int i = 0; i < 500; i++)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->GetTransform()->SetWorldScale(Vector3(0.01f));
		obj->AddComponent(std::make_shared<ModelRenderer>(shader));

		obj->GetModelRenderer()->SetModel(model);

		objs.push_back(obj);
	}

	RenderManager::GetInstance().Init(shader);
}
 
void ModelInstancingDemo::FixedUpdate()
{
}

void ModelInstancingDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void ModelInstancingDemo::PostUpdate()
{
}

void ModelInstancingDemo::PreRender()
{
}

void ModelInstancingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	InstancingManager::GetInstance().Render(objs);
}

void ModelInstancingDemo::PostRender()
{
}

void ModelInstancingDemo::Release()
{
}
