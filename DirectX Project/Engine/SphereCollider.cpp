#include "pch.h"
#include "SphereCollider.h"

#include "BoxCollider.h"

#include "GameObject.h"
#include "Transform.h"

void SphereCollider::Update()
{
	// �ϴ��� transform�� world position�� center��, world scale�� max���� radius�� ����
	// ���߿� ������ ������ ���� ����ϵ��� ����
	auto transform = owner.lock()->GetTransform();

	boundingSphere.Center = transform->GetWorldPosition();
	Vector3 scale = transform->GetWorldScale();
	boundingSphere.Radius = radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(SMRay& ray, OUT float& distance)
{
	return boundingSphere.Intersects(ray.position, ray.direction, distance);
}

bool SphereCollider::Intersects(std::shared_ptr<BaseCollider>& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return boundingSphere.Intersects(std::dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
	case ColliderType::Box:
		return boundingSphere.Intersects(dynamic_pointer_cast<BoxCollider>(other)->GetBoundingBox());
	}

	return false;
}

void SphereCollider::SetRadius(float radius)
{
	this->radius = radius;
}

BoundingSphere& SphereCollider::GetBoundingSphere()
{
	return boundingSphere;
}

SphereCollider::SphereCollider(float radius) : BaseCollider(ColliderType::Sphere)
{
	this->radius = radius;
}

SphereCollider::~SphereCollider()
{
}
