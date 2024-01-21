#pragma once

class SectionNode;

class DebugDrawer;

class ObjectManager
{
//private:
public:
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
	void SpawnObject(Vector3& spawnPoint);
	void ShowObjectPickingUI();
	
	void ObjectPicking(Ray& ray);
	void ShowObjectUI();
	
	void Save(std::wstring sceneFilePath);
	void Load(std::wstring sceneFilePath);

	void Init(std::wstring sceneFilePath);


	//show bounding box
	void ShowBoundingBox(std::shared_ptr<DebugDrawer>& debugDraw);

	ObjectManager();
	~ObjectManager();
};

