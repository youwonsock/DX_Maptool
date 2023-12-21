#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

void Scene::Init()
{
	for(auto& gameObject : gameObjects)
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
	//for (auto& gameObject : gameObjects)
	//	gameObject->Render();

	//instance render
	std::vector<std::shared_ptr<GameObject>> temp;
	temp.insert(temp.end(), gameObjects.begin(), gameObjects.end());
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
