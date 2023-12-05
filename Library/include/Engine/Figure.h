#pragma once

enum class CollisionPos
{
	Overlap,
	Behind,
	Front
};

struct Plane;
struct Cube;
struct Sphere;

struct Cube
{
	union
	{
		Vector3 position = {0,0,0};
		Vector3 center;
	};

	Vector3 size = { 0,0,0 };

	Vector3 min = {0,0,0};
	Vector3 max = {0,0,0};

	CollisionPos ToPlane(Plane& plane);

	void SetCube(Vector3& center, Vector3& size);
	void SetHeight(float minY, float maxY);
};

struct Sphere
{
	union
	{
		Vector3 position = { 0,0,0 };
		Vector3 center;
	};

	float radius = 0.0f;

	CollisionPos ToPlane(Plane& plane);
	void SetRadius(float radius);
};

struct Plane
{
	Vector3 normal = { 0,0,0 };
	Vector3 planeVector = { 0,0,0 };

	Vector3 point[3] = { {0,0,0}, {0,0,0}, {0,0,0} };

	Plane() = default;
	Plane(Vector3& p0, Vector3& p1, Vector3& p2)
	{
		point[0] = p0;
		point[1] = p1;
		point[2] = p2;

		planeVector = p1 - p0;
		Vector3 v2 = p2 - p0;
		
		planeVector.Cross(v2,normal);

		normal.Normalize();
		planeVector.Normalize();
	}

	void SetPlane(Vector3& p0, Vector3& p1, Vector3& p2)
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
};