#include "pch.h"
#include "Picking.h"

void Picking::UpdateRay()
{
	Vector2 mousePos = InputManager::GetInstance().GetMousePos();

	int nX = (int)mousePos.x;
	int nY = (int)mousePos.y;

	ray.origin = Vector3::Zero;
	ray.direction.x = (((2.0f * nX) / (float)Global::g_windowWidth) - 1) / Camera::projectionMatrix._11;
	ray.direction.y = (((-2.0f * nY) / (float)Global::g_windowHeight) + 1) / Camera::projectionMatrix._22;
	ray.direction.z = 1.0f;

	Matrix invertViewMat = Camera::viewMatrix.Invert();

	ray.origin = ray.origin.Transform(ray.origin, invertViewMat);
	ray.direction = ray.direction.TransformNormal(ray.direction, invertViewMat);
	ray.direction.Normalize();
}