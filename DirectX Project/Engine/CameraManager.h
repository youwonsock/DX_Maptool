#pragma once

#include "Singleton.hpp"

class GameObject;
class Camera;

class CameraManager : public Singleton<CameraManager>
{
private:
	friend class Singleton<CameraManager>;
	CameraManager() {};
	~CameraManager() {};

	std::map<std::wstring, std::shared_ptr<GameObject>> cameraMap;
	std::shared_ptr<GameObject> mainCamera;

public:
	bool AddCamera(std::wstring name, std::shared_ptr<GameObject> camera);
	bool SetMainCamera(std::wstring name);
	std::shared_ptr<Camera> GetMainCamera() const;
	std::shared_ptr<Camera> GetCamera(std::wstring name) const;

	void Init();
	void Update();
	void FixedUpdate();
	void PostUpdate();
	void Release();
};

