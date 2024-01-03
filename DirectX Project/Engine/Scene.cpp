#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

void Scene::Init()
{
	for(auto& gameObject : gameObjectsMap)
	{ 
		for (auto& gameObject : gameObject.second)
			gameObject->Init();
	}
}

void Scene::BeginPlay()
{
	for (auto& gameObject : gameObjectsMap)
	{
		for (auto& gameObject : gameObject.second)
			gameObject->BeginPlay();
	}
}

void Scene::FixedUpdate()
{
	for (auto& gameObject : gameObjectsMap)
	{
		for (auto& gameObject : gameObject.second)
			gameObject->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& gameObject : gameObjectsMap)
	{
		for (auto& gameObject : gameObject.second)
			gameObject->Update();
	}
}

void Scene::PostUpdate()
{
	for (auto& gameObject : gameObjectsMap)
	{
		for (auto& gameObject : gameObject.second)
			gameObject->PostUpdate();
	}
}

void Scene::PreRender()
{
	if (drawNodeIdxList.size() > 0)
	{
		for (auto& nodeIdx : drawNodeIdxList)
		{
			for (auto& gameObject : gameObjectsMap[nodeIdx])
				gameObject->PreRender();
		}
	}
	else
	{
		for (auto& gameObject : gameObjectsMap)
		{
			for (auto& gameObject : gameObject.second)
				gameObject->PreRender();
		}
	}
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
	std::set<std::shared_ptr<GameObject>> temp;

	if (drawNodeIdxList.size() > 0)
	{
		for (auto& nodeIdx : drawNodeIdxList)
		{
			for (auto& gameObject : gameObjectsMap[nodeIdx])
			{
				if (!gameObject->isRender)
				{
					temp.insert(gameObject);
					gameObject->isRender = true;
				}
			}
		}
		InstancingManager::GetInstance().Render(temp);
	}
	else
	{
		for (auto& gameObjects : gameObjectsMap)
		{
			for (auto& gameObject : gameObjects.second)
			{
				if (!gameObject->isRender)
				{
					temp.insert(gameObject);
					gameObject->isRender = true;
				}
			}
		}
		InstancingManager::GetInstance().Render(temp);
	}
}

void Scene::PostRender()
{
	if (drawNodeIdxList.size() > 0)
	{
		for (auto& nodeIdx : drawNodeIdxList)
		{
			for (auto& gameObject : gameObjectsMap[nodeIdx])
				gameObject->PostRender();
		}
	}
	else
	{
		for (auto& gameObject : gameObjectsMap)
		{
			for (auto& gameObject : gameObject.second)
				gameObject->PostRender();
		}
	}

	drawNodeIdxList.clear();
}

void Scene::Release()
{
	for (auto& gameObject : gameObjectsMap)
	{
		for (auto& gameObject : gameObject.second)
			gameObject->Release();
	}
}

void Scene::Add(std::shared_ptr<GameObject> gameObject, int nodeIdx)
{
	gameObjectsMap[nodeIdx].insert(gameObject);

	if (gameObject->GetLight() != nullptr)
		lights.insert(gameObject);
}

void Scene::Remove(std::shared_ptr<GameObject> gameObject, int nodeIdx)
{
	gameObjectsMap[nodeIdx].erase(gameObject);

	if (gameObject->GetLight() != nullptr)
		lights.erase(gameObject);
}

void Scene::LoadScene(std::wstring fileName)
{
	// to do load scene data
}
