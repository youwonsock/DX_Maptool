#pragma once

#include "Figure.h"
#include "EnumClass.h"
#include "Component.h"

struct Frustum
{
	Plane planes[6];
	Vector3 frustumCorners[8];

	void Update(Matrix& viewMat, Matrix& projectionMat);
};

class Camera : public Component
{
private:
	using Base = Component;

	ProjectionType projectionType = ProjectionType::Perspective;
	float nearRange = 0.1f;
	float farRange = 1000.0f;

	Frustum frustum;

protected:
	Ray ray;

public:
	Matrix viewMatrix;
	Matrix projectionMatrix;
	Vector3 position;

private:
	void UpdateViewMatrix();

public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectionType projectionType) { this->projectionType = projectionType; };
	void SetNearRange(float nearRange) { this->nearRange = nearRange; };
	void SetFarRange(float farRange) { this->farRange = farRange; };
	
	Frustum& GetFrustum() { return frustum; };
	ProjectionType GetProjectionType() const { return projectionType; };
	Ray& GetRay() { return ray; };
	float GetNearRange() const { return nearRange; };
	float GetFarRange() const { return farRange; };
};

