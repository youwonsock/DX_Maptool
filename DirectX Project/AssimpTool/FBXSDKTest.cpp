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
	
	//camera
	{
		std::shared_ptr<GameObject>cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
	}

	std::shared_ptr<Model> model3 = std::make_shared<Model>();
	obj3 = std::make_shared<GameObject>();
	obj3->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
	obj3->GetTransform()->SetWorldScale(Vector3(1.f));

	//model3->ReadModel(L"Tower/Tower");
	//model3->ReadMaterial(L"Tower/Tower");

	//model3->ReadModel(L"ship/ship");
	//model3->ReadMaterial(L"ship/ship");

	//model3->ReadModel(L"MultiCameras/MultiCameras");
	//model3->ReadMaterial(L"MultiCameras/MultiCameras");
	 
	//obj3->AddComponent(std::make_shared<ModelRenderer>(shader));
	//obj3->GetModelRenderer()->SetModel(model3);
	//obj3->GetModelRenderer()->SetPass(0);

	//turret
	model3->ReadModel(L"Turret_Deploy1/Turret_Deploy1");
	model3->ReadMaterial(L"Turret_Deploy1/Turret_Deploy1");
	model3->ReadAnimation(L"Turret_Deploy1/Turret_Deploy1");

	obj3->AddComponent(std::make_shared<ModelAnimator>(shader));
	obj3->GetModelAnimator()->SetModel(model3);
	obj3->GetModelAnimator()->SetPass(1);

	RenderManager::GetInstance().Init(shader);
}

void FBXSDKTest::FixedUpdate()
{
}

void FBXSDKTest::Update()
{
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

void FBXSDKTest::PostUpdate()
{
}

void FBXSDKTest::PreRender()
{
}

void FBXSDKTest::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	obj3->Render();
}

void FBXSDKTest::PostRender()
{
}

void FBXSDKTest::Release()
{
}
