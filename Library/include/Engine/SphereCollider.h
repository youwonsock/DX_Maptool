#pragma once

#include "BaseCollider.h"

class SphereCollider : public BaseCollider
{
private:
	float radius = 1;
	BoundingSphere boundingSphere;

public:
	virtual void Update() override;
	bool Intersects(SMRay& ray, OUT float& distance) override;
	bool Intersects(std::shared_ptr<BaseCollider>& other) override;

	void SetRadius(float radius);
	BoundingSphere& GetBoundingSphere();

	SphereCollider(float radius);
	virtual ~SphereCollider();
};

