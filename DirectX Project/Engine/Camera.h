#pragma once

#include "EnumClass.h"
#include "Component.h"

class Camera : public Component
{
private:
	using Base = Component;

	ProjectionType projectionType = ProjectionType::Orthographic;

public:
	static Matrix viewMatrix;
	static Matrix projectionMatrix;

public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectionType projectionType) { this->projectionType = projectionType; };
	ProjectionType GetProjectionType() const { return projectionType; };

	void UpdateViewMatrix();
};

