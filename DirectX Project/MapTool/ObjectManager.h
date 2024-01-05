#pragma once

class SectionNode;

class ObjectManager
{
private:
	std::shared_ptr<Shader> shader = nullptr;
	std::shared_ptr<GameObject> pickObject = nullptr;

	std::vector<std::shared_ptr<GameObject>> objectList;
	std::vector<std::string> objectModelNameList;

	int selectedModelIdx = -1;
	int objectCount = 0;
	int selectedObjectIdx = -1;
	bool useFBXSDK = false;

public:


private:
	void ReadObjectModelNameList();

public:
	std::shared_ptr<GameObject> SpawnObject(Vector3& spawnPoint);
	void ObjectPicking(Ray& ray);
	void ShowObjectUI();
	std::shared_ptr<GameObject> ShowObjectPickingUI();
	
	void Init(std::wstring sceneFilePath);

	ObjectManager();
	~ObjectManager();
};

