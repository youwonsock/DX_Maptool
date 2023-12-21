#pragma once

class GameObject;

class Scene
{
private:
	std::unordered_set<std::shared_ptr<GameObject>> gameObjects;
	std::unordered_set<std::shared_ptr<GameObject>> lights;
	
public:
	virtual void Init();
	virtual void BeginPlay();

	virtual void FixedUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	virtual void Release();



	virtual void Add(std::shared_ptr<GameObject> gameObject);
	virtual void Remove(std::shared_ptr<GameObject> gameObject);
};

