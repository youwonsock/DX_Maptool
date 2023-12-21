#include "pch.h"
#include "CameraManager.h"

#include "Camera.h"
#include "GameObject.h"

bool CameraManager::AddCamera(std::wstring name, std::shared_ptr<GameObject> camera)
{
	if (camera->GetCamera() == nullptr)
		return false;

	if (cameraMap.find(name) == cameraMap.end())
	{
		cameraMap.insert(std::make_pair(name, camera));
		return true;
	}

	return false;
}

bool CameraManager::SetMainCamera(std::wstring name)
{
	if (cameraMap.find(name) != cameraMap.end())
	{
		mainCamera = cameraMap[name];
		return true;
	}

	return false;
}

std::shared_ptr<Camera> CameraManager::GetMainCamera() const
{ 
	return mainCamera->GetCamera();
}

std::shared_ptr<Camera> CameraManager::GetCamera(std::wstring name) const
{
	if (cameraMap.find(name) != cameraMap.end())
		return cameraMap.at(name)->GetCamera();

	return nullptr;
}

void CameraManager::Init()
{
	if (cameraMap.size() == 0)
	{
		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
		camera->AddComponent(std::shared_ptr<Camera>());

		AddCamera(L"DefaultCamera", camera);
		SetMainCamera(L"DefaultCamera");
	}
	else
	{
		mainCamera = cameraMap.begin()->second;
	}
}

void CameraManager::Update()
{
	for (auto& camera : cameraMap)
		camera.second->Update();
}

void CameraManager::FixedUpdate()
{
	for (auto& camera : cameraMap)
		camera.second->FixedUpdate();
}

void CameraManager::PostUpdate()
{
	for (auto& camera : cameraMap)
		camera.second->PostUpdate();
}

void CameraManager::Release()
{
	for(auto& camera : cameraMap)
		camera.second->Release();

	cameraMap.clear();
}
