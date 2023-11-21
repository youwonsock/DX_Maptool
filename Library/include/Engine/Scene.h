#pragma once

class GameObject;

class Scene
{
private:
	std::vector<std::shared_ptr<GameObject>> gameObjectArr;

public:
	void Init();
	void BeginPlay();
	void FixedUpdate();
	void Update();
	void PostUpdate();

	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	const std::vector<std::shared_ptr<GameObject>>& GetGameObjectArr() const;
};

