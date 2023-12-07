#include "pch.h"
#include "GameObject.h"

#include "EngineHeader.h"

void GameObject::Init()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->Init();
	}

	for (auto script : scriptArr)
		script->Init();
}

void GameObject::BeginPlay()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->BeginPlay();
	}

	for (auto& script : scriptArr)
		script->BeginPlay();
}

void GameObject::FixedUpdate()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->FixedUpdate();
	}

	for (auto& script : scriptArr)
		script->FixedUpdate();
}

void GameObject::Update()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->Update();
	}

	for (auto& script : scriptArr)
		script->Update();
}

void GameObject::PostUpdate()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->PostUpdate();
	}

	for (auto& script : scriptArr)
		script->PostUpdate();
}

void GameObject::PreRender()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->PreRender();
	}

	for (auto& script : scriptArr)
		script->PreRender();
}

void GameObject::Render()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->Render();
	}

	for (auto& script : scriptArr)
		script->Render();
}

void GameObject::PostRender()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->PostRender();
	}

	for (auto& script : scriptArr)
		script->PostRender();
}

void GameObject::Release()
{
	for (auto& component : componentArr)
	{
		if (component)
			component->Release();
	}

	for (auto& script : scriptArr)
		script->Release();
}

std::shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	UINT index = (UINT)type;
	if (index >= (UINT)ComponentType::FixedComponentCount)
		return nullptr;

	return componentArr[index];
}

std::shared_ptr<Transform> GameObject::GetTransform()
{
	auto& fixedComponent = componentArr[(UINT)ComponentType::Transform];

	if (fixedComponent == nullptr)
	{
		fixedComponent = std::make_shared<Transform>();
		AddComponent(fixedComponent);
	}

	return std::static_pointer_cast<Transform>(fixedComponent);
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	auto& fixedComponent = componentArr[(UINT)ComponentType::Camera];
	return std::static_pointer_cast<Camera>(componentArr[(UINT)ComponentType::Camera]);
}

std::shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	auto& fixedComponent = componentArr[(UINT)ComponentType::MeshRenderer];
	return std::static_pointer_cast<MeshRenderer>(fixedComponent);
}

std::shared_ptr<ModelRenderer> GameObject::GetModelRenderer()
{
	auto& fixedComponent = componentArr[(UINT)ComponentType::ModelRenderer];
	return std::static_pointer_cast<ModelRenderer>(fixedComponent);
}

std::shared_ptr<ModelAnimator> GameObject::GetModelAnimator()
{
	auto& fixedComponent = componentArr[(UINT)ComponentType::Animator];
	return std::static_pointer_cast<ModelAnimator>(fixedComponent);
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	if (component == nullptr)
		return;

	component->SetOwnerGameObject(shared_from_this());

	UINT index = (UINT)component->GetType();
	if (index < (UINT)ComponentType::FixedComponentCount)
		componentArr[index] = component;
	else
		scriptArr.push_back(std::dynamic_pointer_cast<MonoBehaviour>(component));
}
