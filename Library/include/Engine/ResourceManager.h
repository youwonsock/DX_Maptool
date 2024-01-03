#pragma once

#include "ResourceBase.h"
#include "Singleton.hpp"

class Mesh;
class Shader;
class Texture;
class Material;
class Model;

class ResourceManager : public Singleton<ResourceManager>
{
private:
	friend class Singleton<ResourceManager>;
	ResourceManager() {};

	ComPtr<ID3D11Device> device;
	std::wstring resourcePath = L"../../Res/";

	using KeyObjMap = std::unordered_map<std::wstring, std::shared_ptr<ResourceBase>>;
	std::array<KeyObjMap, static_cast<size_t>(ResourceType::RecourcesCount)> resourceMapArr;

private:
	void CreateDefaultMesh();
public:
	void Init();

	template <typename T>
	std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path, bool useResourcePath = true);

	template<typename T>
	void Add(const std::wstring& key, std::shared_ptr<T> object);

	template<typename T>
	std::shared_ptr<T> Get(const std::wstring& key);

	template<typename T>
	ResourceType GetResourceType();
};

template<typename T>
inline std::shared_ptr<T> ResourceManager::Load(const std::wstring& key, const std::wstring& path, bool useResourcePath)
{
	auto objectType = GetResourceType<T>();
	auto& resourceMap = resourceMapArr[static_cast<size_t>(objectType)];
	std::wstring fullPath;

	if (useResourcePath && path.find(L"../../Res/") == std::wstring::npos)
		fullPath = resourcePath + path;
	else
		fullPath = path;

	auto iter = resourceMap.find(key);
	if (iter != resourceMap.end())
	{
		if (iter->second->GetPath() != fullPath)
		{
			if (!iter->second->Load(fullPath))
				return nullptr;
		}

		return std::dynamic_pointer_cast<T>(iter->second);
	}

	std::shared_ptr<T> resource = std::make_shared<T>();

	if (!resource->Load(fullPath))
		return nullptr;
	
	resourceMap.insert({ key, resource });

	return resource;
}

template<typename T>
inline void ResourceManager::Add(const std::wstring& key, std::shared_ptr<T> object)
{
	auto resourceType = GetResourceType<T>();
	auto& resourceMap = resourceMapArr[static_cast<size_t>(resourceType)];

	auto iter = resourceMap.find(key);
	if (iter != resourceMap.end())
		return;

	resourceMap.insert({ key, object });
	return;                                 
}

template<typename T>
inline std::shared_ptr<T> ResourceManager::Get(const std::wstring& key)
{
	auto resourceType = GetResourceType<T>();
	auto& keyObjMap = resourceMapArr[static_cast<size_t>(resourceType)];

	auto iter = keyObjMap.find(key);
	if (iter != keyObjMap.end())
		return std::static_pointer_cast<T>(iter->second);

	return nullptr;
}

template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Model>)
		return ResourceType::Model;


	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;

	assert(false);
	return ResourceType::Unknown;
}
