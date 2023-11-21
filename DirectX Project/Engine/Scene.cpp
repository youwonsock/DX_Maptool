#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

void Scene::Init()
{
	for (auto& gameObject : gameObjectArr)
		gameObject->Init();
}

void Scene::BeginPlay()
{
	for (auto& gameObject : gameObjectArr)
		gameObject->BeginPlay();
}

void Scene::FixedUpdate()
{
	for (auto& gameObject : gameObjectArr)
		gameObject->FixedUpdate();
}

void Scene::Update()
{
	for (auto& gameObject : gameObjectArr)
		gameObject->Update();
}

void Scene::PostUpdate()
{
	for (auto& gameObject : gameObjectArr)
		gameObject->PostUpdate();
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjectArr.push_back(gameObject);
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	auto target = std::find(gameObjectArr.begin(), gameObjectArr.end(), gameObject);

	if(target != gameObjectArr.end())
		gameObjectArr.erase(target);
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjectArr() const
{
	return gameObjectArr;
}
