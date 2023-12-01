#include "pch.h"
#include "ResourceManager.h"

#include "ResourceHeader.h"
#include "PipeLineHeaders.h"

void ResourceManager::CreateDefaultMesh()
{
	std::shared_ptr<Mesh> mesh1 = std::make_shared<Mesh>();
	mesh1->CreateCube();
	Add(L"Cube", mesh1);

	std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>();
	mesh2->CreateRactangle();
	Add(L"Quad", mesh2);

	std::shared_ptr<Mesh> mesh3 = std::make_shared<Mesh>();
	mesh3->CreateSphere();
	Add(L"Sphere", mesh3);
}

void ResourceManager::Init()
{
	CreateDefaultMesh();

	// make default Texture;
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Load(L"../../Res/Textures/Default.png");
	Add(L"Default", texture);
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::wstring& key, const std::wstring& path)
{
	std::shared_ptr<Texture> texture = Get<Texture>(key);

	if (texture == nullptr)
	{
		texture = std::make_shared<Texture>();
		texture->Load(path);
		Add<Texture>(key, texture);
	}

	return texture;
}
