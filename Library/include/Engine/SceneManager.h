#pragma once

#include "Singleton.hpp"

class Scene;

class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton<SceneManager>;
	SceneManager() {};

	std::shared_ptr<Scene> currentScene;

public:
	void Init();
	void Update();

	void LoadScene(std::wstring sceneName);

	const std::shared_ptr<Scene> GetCurrentScene() const;
};

