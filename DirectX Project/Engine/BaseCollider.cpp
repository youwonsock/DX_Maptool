#include "pch.h"
#include "BaseCollider.h"

BaseCollider::BaseCollider(ColliderType colliderType) : Component(ComponentType::Collider), colliderType(colliderType)
{
}

BaseCollider::~BaseCollider()
{
}
