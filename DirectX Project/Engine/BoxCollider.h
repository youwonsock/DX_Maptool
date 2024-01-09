#pragma once

#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
private:
	BoundingOrientedBox boundingBox;

public:
	BoundingOrientedBox& GetBoundingBox() { return boundingBox; }

	virtual void Update() override;
	bool Intersects(SMRay& ray, OUT float& distance) override;
	bool Intersects(std::shared_ptr<BaseCollider>& other) override;

	BoxCollider();
	virtual ~BoxCollider();
};

