#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

#include "Collision.h"
#include "Transform.h"
#include "Camera.h"

void Scene::Init()
{
	for (auto& gameObject : gameObjects)
		gameObject->Init();
}

void Scene::BeginPlay()
{
	for (auto& gameObject : gameObjects)
		gameObject->BeginPlay();
}

void Scene::FixedUpdate()
{
	for (auto& gameObject : gameObjects)
		gameObject->FixedUpdate();
}

void Scene::Update()
{
	for (auto& gameObject : gameObjects)
		gameObject->Update();
}

void Scene::PostUpdate()
{
	for (auto& gameObject : gameObjects)
		gameObject->PostUpdate();
}

void Scene::PreRender()
{
	for (auto& gameObject : gameObjects)
		gameObject->PreRender();
}

void Scene::Render()
{
	/*for (auto& gameObjects : gameObjectsMap)
	{
		for (auto& gameObject : gameObjects.second)
			gameObject->Render();
	}
	return;*/

	//instance render
	std::vector<std::shared_ptr<GameObject>> temp;
	auto& frustum =	CameraManager::GetInstance().GetMainCamera()->GetFrustum();

	for (auto& gameObject : gameObjects)
	{
		bool isDraw = true;

		for (int j = 0; j < 6; ++j)
		{
			if (!Collision::CubeToPlane(gameObject->GetTransform()->GetBoundingBox(), frustum.planes[j]))
			{
				isDraw = false;
				break;
			}
		}

		if (isDraw)
			temp.push_back(gameObject);
	}
	
	InstancingManager::GetInstance().Render(temp);
}

void Scene::PostRender()
{
	for (auto& gameObject : gameObjects)
		gameObject->PostRender();
}

void Scene::Release()
{
	for (auto& gameObject : gameObjects)
		gameObject->Release();
}

void Scene::Add(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.insert(gameObject);

	if (gameObject->GetLight() != nullptr)
		lights.insert(gameObject);
}

void Scene::Remove(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.erase(gameObject);

	if (gameObject->GetLight() != nullptr)
		lights.erase(gameObject);
}

void Scene::ClearScene()
{
	gameObjects.clear();
	lights.clear();
}

