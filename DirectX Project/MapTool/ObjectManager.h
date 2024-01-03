#pragma once

class SectionNode;

class ObjectManager
{
private:
	std::shared_ptr<Scene> scene = nullptr;
	std::shared_ptr<Shader> shader = nullptr;

public:


private:


public:
	void Init(std::wstring sceneFilePath, std::wstring shaderFilePath);

	void SpawnObject(const std::wstring& objectModelName, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap);

	ObjectManager();
	~ObjectManager();
};

