#include "pch.h"
#include "BoxCollider.h"

#include "SphereCollider.h"

void BoxCollider::Update()
{
}

bool BoxCollider::Intersects(SMRay& ray, OUT float& distance)
{
	return boundingBox.Intersects(ray.position,ray.direction, distance);
}

bool BoxCollider::Intersects(std::shared_ptr<BaseCollider>& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return boundingBox.Intersects(std::dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
	case ColliderType::Box:
		return boundingBox.Intersects(dynamic_pointer_cast<BoxCollider>(other)->GetBoundingBox());
	}

	return false;
}

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{
}

BoxCollider::~BoxCollider()
{
}
