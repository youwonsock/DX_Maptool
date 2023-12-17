#pragma once

#include "EnumClass.h"

class MonoBehaviour;
class Component;
class Transform;
class Camera;
class MeshRenderer;
class ModelRenderer;
class ModelAnimator;
class Light;

class GameObject : public std::enable_shared_from_this<GameObject>
{
protected:
	std::array<std::shared_ptr<Component>, (UINT)ComponentType::FixedComponentCount> componentArr = { nullptr, };
	std::vector<std::shared_ptr<MonoBehaviour>> scriptArr;

public:
	virtual void Init();
	virtual void BeginPlay();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual void Release();

	
	std::shared_ptr<Component> GetFixedComponent(ComponentType type);
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Camera> GetCamera();
	std::shared_ptr<MeshRenderer> GetMeshRenderer();
	std::shared_ptr<ModelRenderer> GetModelRenderer();
	std::shared_ptr<ModelAnimator> GetModelAnimator();
	std::shared_ptr<Light> GetLight();

	void AddComponent(std::shared_ptr<Component> component);
};

