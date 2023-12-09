#pragma once

#include "Figure.h"
#include "EnumClass.h"
#include "Component.h"

class Camera : public Component
{
private:
	using Base = Component;

	ProjectionType projectionType = ProjectionType::Perspective;

	float nearRange = 0.1f;
	float farRange = 1000.0f;

public:
	static Matrix viewMatrix;
	static Matrix projectionMatrix;
	static Vector3 position;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectionType projectionType) { this->projectionType = projectionType; };
	ProjectionType GetProjectionType() const { return projectionType; };

	void UpdateViewMatrix();
};

