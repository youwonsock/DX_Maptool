#include "pch.h"
#include "TweenDemo.h"
#include "Engine/TestCamera.h"

#include "Engine/Model.h"
#include "Engine/ModelRenderer.h"
#include "Engine/RenderManager.h"
#include "Engine/GameObject.h"

void TweenDemo::CreateKachujin()
{
	std::shared_ptr <class Model> model = std::make_shared<Model>();
	model->ReadModel(L"Kachujin/Kachujin");
	model->ReadMaterial(L"Kachujin/Kachujin");
	model->ReadAnimation(L"Kachujin/Idle");
	model->ReadAnimation(L"Kachujin/Run");
	model->ReadAnimation(L"Kachujin/Slash");
	
	obj = std::make_shared<GameObject>();
	obj->GetTransform()->SetWorldPosition(Vector3(0, 0, 1));
	obj->GetTransform()->SetWorldScale(Vector3(0.01f));
	
	obj->AddComponent(std::make_shared<ModelAnimator>(shader));
	{
		obj->GetModelAnimator()->SetModel(model);
		//obj->GetModelAnimator()->SetPass(1);
	}
}

void TweenDemo::Init()
{
	shader = std::make_shared<Shader>(L"17. TweenDemo.fx");

	camera = std::make_shared<GameObject>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 0, -5));
	camera->AddComponent(std::make_shared<Camera>());
	camera->AddComponent(std::make_shared<TestCamera>());

	CreateKachujin();

	RenderManager::GetInstance().Init(shader);
}

void TweenDemo::FixedUpdate()
{
}

void TweenDemo::Update()
{
	camera->Update();
	RenderManager::GetInstance().Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(0.4f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(0.f);
		lightDesc.direction = Vector3(1.f, 0.f, 1.f);
		RenderManager::GetInstance().PushLightData(lightDesc);
	}
}

void TweenDemo::PostUpdate()
{
}

void TweenDemo::PreRender()
{
}

void TweenDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj->Update();
}

void TweenDemo::PostRender()
{
}

void TweenDemo::Release()
{
}
