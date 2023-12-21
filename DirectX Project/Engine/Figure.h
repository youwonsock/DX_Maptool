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
struct Rect;
struct Ray;

// --------------------------------- Ray ---------------------------------//

struct Ray
{
	Vector3		origin = {0,0,0};
	Vector3		direction = { 0,0,0 };

	void UpdateRay(const Matrix& viewMat, const Matrix& projectionMat);
};

// --------------------------------- Cube ---------------------------------//

struct Cube
{
	union
	{
		Vector3 position = {0,0,0};
		Vector3 center;
	};

	Vector3 min = {0,0,0};
	Vector3 max = {0,0,0};
	Vector3 axisVector[3] = { {1,0,0}, {0,1,0}, {0,0,1} };
	Vector3 size = { 0,0,0 };

	Cube() {};
	Cube(Vector3& center, float x, float y, float z);
	Cube(Vector3& min, Vector3& max);

	void SetCube(Vector3& center, float x, float y, float z);
	void SetCube(Vector3& min, Vector3& max);
	void SetHeight(float minY, float maxY);
};

// --------------------------------- Plane ---------------------------------//

struct Plane
{
	Vector4 normal = { 0,0,0,0 };

	Plane() {};
	Plane(Vector3& p0, Vector3& p1, Vector3& p2);
	void SetPlane(Vector3& p0, Vector3& p1, Vector3& p2);
};


// --------------------------------- Circle ---------------------------------//

struct Circle
{
	union
	{
		Vector2 center;
		Vector2 position;
	};

	float radius = 0.0f;

	Circle(Vector2 center, float radius);
	void SetCircle(Vector2 center, float radius);
};