#include "pch.h"

#include "Component.h"
#include "GameObject.h"

Component::Component(ComponentType componentType) : type(componentType)
{

}

Component::~Component()
{

}

std::shared_ptr<GameObject> Component::GetOwnerGameObject()
{
	auto ownerGameObj = owner.lock();

	if(ownerGameObj)
		return ownerGameObj;

	return nullptr;
}

std::shared_ptr<Transform> Component::GetTransform()
{
	auto ownerGameObj = owner.lock();

	if (ownerGameObj)
		return ownerGameObj->GetTransform();

	return nullptr;
}
