#pragma once

struct Plane;
struct Cube;
struct Sphere;
struct Rect;
struct Ray;
struct Circle;

class Collision
{
private:
	static bool SeperatingPlane(const Vector3& pos, const Vector3& dir, const Cube& cube1, const Cube& cube2);

public:
	// --------------------------------- Ray ---------------------------------//
	static bool RayToFace(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* pickPoint = nullptr);

	// --------------------------------- Cube ---------------------------------//

	static bool CubeToRay(const Cube& cube, const Ray& ray);
	static bool CubeToPlane(const Cube& cube, const Plane& plane);
	static bool CubeToCube(const Cube& cube1, const Cube& cube2);

	// --------------------------------- Circle ---------------------------------//
	static bool CircleToPoint(const Circle& circle, const Vector3& point);
};

