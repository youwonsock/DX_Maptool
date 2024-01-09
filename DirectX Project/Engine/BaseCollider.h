#pragma once

#include "Component.h"

enum class ColliderType
{
	Sphere,
	Box
};

class BaseCollider : public Component
{
private:
	ColliderType colliderType;

public:
	virtual bool Intersects(SMRay& ray, OUT float& distance) = 0;
	virtual bool Intersects(std::shared_ptr<BaseCollider>& other) = 0;

	ColliderType GetColliderType() { return colliderType; }
	
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();
};

