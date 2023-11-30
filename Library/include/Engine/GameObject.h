#pragma once

#include "EnumClass.h"

class MonoBehaviour;
class Component;
class Transform;
class Camera;
class MeshRenderer;
class ModelRenderer;

class GameObject : public std::enable_shared_from_this<GameObject>
{
protected:
	std::array<std::shared_ptr<Component>, (UINT)ComponentType::FixedComponentCount> componentArr = { nullptr, };
	std::vector<std::shared_ptr<MonoBehaviour>> scriptArr;

public:
	void Init();
	void BeginPlay();
	void FixedUpdate();
	void Update();
	void PostUpdate();
	void Release();
	
	std::shared_ptr<Component> GetFixedComponent(ComponentType type);
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Camera> GetCamera();
	std::shared_ptr<MeshRenderer> GetMeshRenderer();
	std::shared_ptr<ModelRenderer> GetModelRenderer();

	void AddComponent(std::shared_ptr<Component> component);
};

