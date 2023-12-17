#pragma once

#include "Singleton.hpp"

#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton<SceneManager>;
	SceneManager() {};

	std::shared_ptr<Scene> currentScene = std::make_shared<Scene>();

public:
	~SceneManager() {};

	void Init();
	void BeginPlay();

	void FixedUpdate();
	void Update();
	void PostUpdate();

	void PreRender();
	void Render();
	void PostRender();

	void Release();

	template<typename T>
	void ChangeScene(std::shared_ptr<T> scene)
	{
		currentScene->Release();

		currentScene = scene;
		currentScene->Init();
		currentScene->BeginPlay();
	}

	std::shared_ptr<Scene> GetCurrentScene() { return currentScene; }
};

