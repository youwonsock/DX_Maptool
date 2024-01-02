#pragma once

class GameObject;

class Scene
{
private:
	 std::unordered_map<int,std::unordered_set<std::shared_ptr<GameObject>>> gameObjectsMap;

	 std::unordered_set<std::shared_ptr<GameObject>> lights;


	 std::vector<int> drawNodeIdxList;
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

	virtual void SetDrawNodeIdxList(std::vector<int>& drawNodeIdxList) { this->drawNodeIdxList = drawNodeIdxList; }
	virtual void Add(std::shared_ptr<GameObject> gameObject, int nodeIdx = -1);
	virtual void Remove(std::shared_ptr<GameObject> gameObject, int nodeIdx = -1);

	virtual void LoadScnene(std::wstring fileName);
};

