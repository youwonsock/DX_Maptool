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

bool Picking::PickTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;

	Vector3 pvec = ray.direction.Cross(edge2);
	float det = edge1.Dot(pvec);

	Vector3 tvec = Vector3::Zero;
	if (det > 0.0f) // shot front
	{
		tvec = ray.origin - v0;
	}
	else            // shot back
	{
		tvec = v0 - ray.origin;
		det = -det;
	}

	if (det < 0.0001f)	// parallel
		return false;

	// get u (v0 -> v1)
	float u = tvec.Dot(pvec);
	if (u < 0.0f || u > det)
		return false;

	// get v (v0 -> v2)
	Vector3 qvec = tvec.Cross(edge1);
	float v = ray.direction.Dot(qvec);
	if (v < 0.0f || u + v > det)
		return false;

	// get t (origin -> v0)
	// currentlly, t is not nercessary
	float t = edge2.Dot(qvec);
	float fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	return true;
}
