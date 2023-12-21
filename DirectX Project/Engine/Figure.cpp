#include "pch.h"
#include "Figure.h"

// --------------------------------- Ray ---------------------------------//

void Ray::UpdateRay(const Matrix& viewMat, const Matrix& projectionMat)
{
	Vector2 mousePos = InputManager::GetInstance().GetMousePos();

	int nX = (int)mousePos.x;
	int nY = (int)mousePos.y;

	origin = Vector3::Zero;
	direction.x = (((2.0f * nX) / (float)Global::g_windowWidth) - 1) / projectionMat._11;
	direction.y = (((-2.0f * nY) / (float)Global::g_windowHeight) + 1) / projectionMat._22;
	direction.z = 1.0f;

	Matrix invertViewMat = viewMat.Invert();

	origin = origin.Transform(origin, invertViewMat);
	direction = direction.TransformNormal(direction, invertViewMat);
	direction.Normalize();
}

// --------------------------------- Cube ---------------------------------//

Cube::Cube(Vector3& center, float x, float y, float z)
{
	this->center = center;	
	size = Vector3(x, y, z);

	min = center - size * 0.5f;
	max = center + size * 0.5f;

	Vector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

Cube::Cube(Vector3& min, Vector3& max)
{
	this->min = min;
	this->max = max;

	center = (max + min) * 0.5f;
	size = max - min;

	Vector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetCube(Vector3& center, float x, float y, float z)
{
	this->center = center;
	size = Vector3(x, y, z);

	min = center - size * 0.5f;
	max = center + size * 0.5f;

	Vector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetCube(Vector3& min, Vector3& max)
{
	this->min = min;
	this->max = max;

	center = (max + min) * 0.5f;
	size = max - min;

	Vector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetHeight(float minY, float maxY)
{
	min.y = minY;
	max.y = maxY;
	center.y = (minY + maxY) * 0.5f;
	size.y = maxY - minY;

	Vector3 c0, c2;
	c2 = min;
	c0 = c2; c0.y += size.y;
	axisVector[1] = (c0 - c2) * 0.5f;
}

// --------------------------------- Plane ---------------------------------//


Plane::Plane(Vector3& p0, Vector3& p1, Vector3& p2)
{
	Vector3 v1 = p1 - p0;
	Vector3 v2 = p2 - p0;

	Vector3 tempNormal;
	v1.Cross(v2, tempNormal);

	tempNormal.Normalize();
	normal = Vector4(tempNormal.x, tempNormal.y, tempNormal.z, 0.0f);

	normal.w = -(normal.x * p0.x + normal.y * p0.y + normal.z * p0.z);
}

void Plane::SetPlane(Vector3& p0, Vector3& p1, Vector3& p2)
{
	Vector3 v1 = p1 - p0;
	Vector3 v2 = p2 - p0;

	Vector3 tempNormal;
	v1.Cross(v2, tempNormal);

	tempNormal.Normalize();
	normal = Vector4(tempNormal.x, tempNormal.y, tempNormal.z, 0.0f);

	normal.w = -(normal.x * p0.x + normal.y * p0.y + normal.z * p0.z);
}

// --------------------------------- Circle ---------------------------------//

Circle::Circle(Vector2 center, float radius)
{
	this->center = center;
	this->radius = radius;
}

void Circle::SetCircle(Vector2 center, float radius)
{
	this->center = center;
	this->radius = radius;
}