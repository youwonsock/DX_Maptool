#include "pch.h"
#include "Figure.h"

CollisionPos Cube::ToPlane(Plane& plane)
{
	return CollisionPos();
}

void Cube::SetCube(Vector3& center, Vector3& size)
{
}

void Cube::SetHeight(float minY, float maxY)
{
}

CollisionPos Sphere::ToPlane(Plane& plane)
{
	return CollisionPos();
}

void Sphere::SetRadius(float radius)
{
}
