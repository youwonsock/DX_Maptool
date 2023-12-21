#include "pch.h"
#include "MapTool.h"

#include "Engine/TestCamera.h"

#include "Terrain.h"
#include "SkyBox.h"

void MapTool::Init()
{
	{
		TerrainDesc info;
		
		info.rowNum = 129;
		info.colNum = 129;
		info.cellDistance = 1;
		info.heightScale = 0.25f;
		info.shaderFilePath = L"MapToolShader/MapToolShader.fx";
		info.heightMapFilePath = L"../../Res/Textures/Terrain/height129.PNG";
		info.alphaTexPath = L"../../Res/Textures/Terrain/heightExported.PNG";
		info.textureFilePath = L"../../Res/Textures/Terrain/grass.jpg";
		info.DevideTreeDepth = 2;

		map = std::make_shared<GameObject>();
		map->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		map->AddComponent(std::make_shared<Terrain>(info));

	}

	// skybox
	{
		map->AddComponent(std::make_shared<SkyBox>());
	}
		
	map->Init();

	// camera
	{
		cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
	}


	//// object
	//{
	//	objectShader = std::make_shared<Shader>(L"23. RenderDemo.fx");

	//	// anim
	//	std::shared_ptr<Model> model1 = std::make_shared<Model>();
	//	model1->ReadModel(L"Kachujin/Kachujin");
	//	model1->ReadMaterial(L"Kachujin/Kachujin");
	//	model1->ReadAnimation(L"Kachujin/Idle");
	//	model1->ReadAnimation(L"Kachujin/Run");
	//	model1->ReadAnimation(L"Kachujin/Slash");

	//	for (int i = 0; i < 10; ++i)
	//	{
	//		auto obj = std::make_shared<GameObject>();
	//		obj->GetTransform()->SetWorldPosition(Vector3(64 - rand() % 128, 5, 64 - rand() % 128));
	//		obj->GetTransform()->SetWorldScale(Vector3(0.05f));

	//		//set y pos
	//		{
	//			std::shared_ptr<Terrain> terrain = std::dynamic_pointer_cast<Terrain>(map->GetFixedComponent(ComponentType::Terrain));
	//			int x = obj->GetTransform()->GetWorldPosition().x;
	//			int z = obj->GetTransform()->GetWorldPosition().z;

	//			obj->GetTransform()->SetWorldPosition(Vector3(x, terrain->GetHeightMap(z + 64,x + 64), z));
	//		}

	//		obj->AddComponent(std::make_shared<ModelAnimator>(objectShader));

	//		{
	//			obj->GetModelAnimator()->SetModel(model1);
	//			obj->GetModelAnimator()->SetPass(2);
	//		}

	//		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//	}

	//	// model
	//	std::shared_ptr<Model> model2 = std::make_shared<Model>();
	//	model2->ReadModel(L"Tower/Tower");
	//	model2->ReadMaterial(L"Tower/Tower");

	//	for (int i = 0; i < 10; i++)
	//	{
	//		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	//		obj->GetTransform()->SetWorldPosition(Vector3(64 - rand() % 128, 5, 64 - rand() % 128));
	//		obj->GetTransform()->SetWorldScale(Vector3(0.01f));

	//		//set y pos
	//		{
	//			std::shared_ptr<Terrain> terrain = std::dynamic_pointer_cast<Terrain>(map->GetFixedComponent(ComponentType::Terrain));
	//			int x = obj->GetTransform()->GetWorldPosition().x;
	//			int z = obj->GetTransform()->GetWorldPosition().z;

	//			obj->GetTransform()->SetWorldPosition(Vector3(x, terrain->GetHeightMap(z + 64, x + 64), z));
	//		}

	//		obj->AddComponent(std::make_shared<ModelRenderer>(objectShader));

	//		obj->GetModelRenderer()->SetModel(model2);
	//		obj->GetModelRenderer()->SetPass(1);

	//		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	//	}
	//}

	//RenderManager::GetInstance().Init(objectShader);
}

void MapTool::FixedUpdate()
{
	map->FixedUpdate();
}

void MapTool::Update()
{
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	map->Update();
}

void MapTool::PostUpdate()
{
	map->PostUpdate();

}

void MapTool::PreRender()
{
	map->PreRender();
}

void MapTool::Render()
{
	map->Render();
}

void MapTool::PostRender()
{
	map->PostRender();
}

void MapTool::Release()
{
	map->Release();

}