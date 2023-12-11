#include "pch.h"
#include "Figure.h"

Cube::Cube(Vector3& center, float x, float y, float z)
{
	this->center = center;	
	size = Vector3(x, y, z);

	min = center - size / 2.0f;
	max = center + size / 2.0f;

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

	center = (max + min) / 2.0f;
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

	min = center - size / 2.0f;
	max = center + size / 2.0f;

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

	center = (max + min) / 2.0f;
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
	center.y = (minY + maxY) / 2.0f;
	size.y = maxY - minY;

	Vector3 c0, c2;
	c2 = min;
	c0 = c2; c0.y += size.y;
	axisVector[1] = (c0 - c2) * 0.5f;
}

CollisionPos Cube::ToPlane(Plane& plane)
{
	//// not working
	Vector3 cubePosVec = center - plane.point[0];
	Vector3 distanceVec = cubePosVec - (plane.planeVector * (cubePosVec.Dot(plane.planeVector)));
	
	float sum = axisVector[0].Dot(plane.normal) + axisVector[1].Dot(plane.normal) + axisVector[1].Dot(plane.normal);
	
	if (fabs(sum) >= fabs(distanceVec.Dot(plane.normal)))
		return CollisionPos::Overlap;
	
	if (0 > (distanceVec.Dot(plane.normal)))
		return CollisionPos::Behind;
	
	return CollisionPos::Front;	

	/*float fDist = 0.0f;
	float fPlaneToCenter = 0.0f;
	plane.normal.Normalize();

	Vector3 vDir;
	float fA, fB, fC, fD;
	fA = plane.normal.x;
	fB = plane.normal.y;
	fC = plane.normal.z;

	Vector3 v0 = plane.point[0];
	fD = -(fA * v0.x + fB * v0.y + fC * v0.z);

	vDir = Vector3(1,0,0) * (max.x - center.x);
	fDist += fabs(fA * vDir.x + fB * vDir.y + fC * vDir.z);
	vDir = Vector3(0, 1, 0) * (max.y - center.y);
	fDist += fabs(fA * vDir.x + fB * vDir.y + fC * vDir.z);
	vDir = Vector3(0, 0, 1) * (max.z - center.z);
	fDist += fabs(fA * vDir.x + fB * vDir.y + fC * vDir.z);

	fPlaneToCenter = fA * center.x + fB * center.y + fC * center.z + fD;
	if (fPlaneToCenter < -fDist)
	{
		return CollisionPos::Behind;
	}

	return CollisionPos::Overlap;*/
}

//--------------------------------- Sphere ---------------------------------//

CollisionPos Sphere::ToPlane(Plane& plane)
{
	return CollisionPos();
}

void Sphere::SetRadius(float radius)
{
}

// --------------------------------- Plane ---------------------------------//


Plane::Plane(Vector3& p0, Vector3& p1, Vector3& p2)
{
	point[0] = p0;
	point[1] = p1;
	point[2] = p2;

	planeVector = p1 - p0;
	Vector3 v2 = p2 - p0;

	planeVector.Cross(v2, normal);

	normal.Normalize();
	planeVector.Normalize();
}

void Plane::SetPlane(Vector3& p0, Vector3& p1, Vector3& p2)
{
	point[0] = p0;
	point[1] = p1;
	point[2] = p2;

	planeVector = p1 - p0;
	Vector3 v2 = p2 - p0;

	planeVector.Cross(v2, normal);

	normal.Normalize();
	planeVector.Normalize();
}

// --------------------------------- Square ---------------------------------//


bool Square::operator == (const Square& v)
{
	if (fabs((min - v.min).Length()) < 0.0001f)
	{
		if (fabs((max - v.max).Length()) < 0.0001f)
		{
			return true;
		}
	}
	return false;
}

Square::Square(Vector2 min, Vector2 max)
{
	this->min = min;
	this->max = max;
	center = (max + min);
	center /= 2.0f;
	size.x = max.x - min.x;
	size.y = max.y - min.y;
}
Square::Square(Vector2 v, float w, float h)
{
	center = v;
	this->min = center - Vector2(w / 2.0f, h / 2.0f);
	this->max = center + Vector2(w / 2.0f, h / 2.0f);
	this->size.x = w;
	this->size.y = h;
}

void Square::SetSquare(Vector2 min, Vector2 max)
{
	this->min = min;
	this->max = max;
	center = (max + min);
	center /= 2.0f;
	size.x = max.x - min.x;
	size.y = max.y - min.y;
}

void Square::SetSquare(Vector2 v, float w, float h)
{
	center = v;
	this->min = center - Vector2(w / 2.0f, h / 2.0f);
	this->max = center + Vector2(w / 2.0f, h / 2.0f);
	this->size.x = w;
	this->size.y = h;
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

bool Circle::ToPoint(Vector2 point)
{
	if (fabs((center - point).Length()) < radius)
		return true;

	return false;
}
