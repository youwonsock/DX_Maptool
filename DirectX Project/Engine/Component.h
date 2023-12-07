#pragma once

#include "EnumClass.h"

class GameObject;
class Transform;

class Component
{
public:
	Component(ComponentType componentType);
	virtual ~Component();

	virtual void Init()			{};
	virtual void BeginPlay()	{};
	virtual void FixedUpdate()	{};
	virtual void Update()		{};
	virtual void PostUpdate()	{};
	virtual void PreRender() {};
	virtual void Render() {};
	virtual void PostRender() {};
	virtual void Release()		{};

public:
	std::shared_ptr<GameObject> GetOwnerGameObject();
	std::shared_ptr<Transform>  GetTransform();
	ComponentType& GetType() { return type; };

private:
	friend class GameObject;
	void SetOwnerGameObject(std::shared_ptr<GameObject> owner) { this->owner = owner; };

protected:
	ComponentType type;
	std::weak_ptr<GameObject> owner;
};

