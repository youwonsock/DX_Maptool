#pragma once

class SectionNode;

class ObjectManager
{
private:
	std::shared_ptr<Scene> scene = nullptr;
	std::shared_ptr<Shader> shader = nullptr;
	std::wstring sceneFilePath;

public:


private:


public:
	void Init(std::wstring sceneFilePath, std::wstring shaderFilePath);

	void SpawnObject(std::wstring& objectModelPath, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap);

	ObjectManager();
	~ObjectManager();
};

