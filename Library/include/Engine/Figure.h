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

	CollisionPos ToPlane(Plane& plane);

	Cube() {};
	Cube(Vector3& center, float x, float y, float z);
	Cube(Vector3& min, Vector3& max);

	void SetCube(Vector3& center, float x, float y, float z);
	void SetCube(Vector3& min, Vector3& max);
	void SetHeight(float minY, float maxY);
};

// --------------------------------- Sphere ---------------------------------//

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

// --------------------------------- Plane ---------------------------------//

struct Plane
{
	Vector3 normal = { 0,0,0 };
	Vector3 planeVector = { 0,0,0 };
	Vector3 point[3] = { {0,0,0}, {0,0,0}, {0,0,0} };

	Plane() {};
	Plane(Vector3& p0, Vector3& p1, Vector3& p2);
	void SetPlane(Vector3& p0, Vector3& p1, Vector3& p2);
};

// --------------------------------- Square ---------------------------------//

struct Square
{
	union
	{
		Vector2 center;
		Vector2 position;
	};

	// 0   1
	//
	// 3   2
	Vector2 min;
	Vector2 max;
	Vector2 size;

	bool operator == (const Square& v);

	Square(Vector2 min, Vector2 max);
	Square(Vector2 v, float w, float h);
	void SetSquare(Vector2 min, Vector2 max);
	void SetSquare(Vector2 v, float w, float h);
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

	bool ToPoint(Vector2 point);
};