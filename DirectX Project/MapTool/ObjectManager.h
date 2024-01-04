#pragma once

class SectionNode;

class ObjectManager
{
private:
	std::shared_ptr<Scene> scene = nullptr;
	std::shared_ptr<Shader> shader = nullptr;

	std::vector<std::shared_ptr<GameObject>> objectList;
	std::vector<std::string> objectModelNameList;
	int selectedModelIdx = -1;

	std::shared_ptr<GameObject> pickObject = nullptr;

public:


private:
	void ReadObjectModelNameList();

public:
	void SpawnObject(const std::wstring& objectModelName, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap);
	void ShowUI();
	
	void Init(std::wstring sceneFilePath);

	ObjectManager();
	~ObjectManager();
};

