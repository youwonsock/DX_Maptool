#pragma once

struct Plane;
struct Cube;
struct Sphere;
struct Rect;
struct Ray;
struct Circle;

class Collision
{
public:
	// --------------------------------- Ray ---------------------------------//
	static bool RayToFace(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* pickPoint = nullptr);

	// --------------------------------- Cube ---------------------------------//

	static bool CubeToRay(const Cube& cube, const Ray& ray);
	static bool CubeToPlane(const Cube& cube, const Plane& plane);

	// --------------------------------- Circle ---------------------------------//
	static bool CircleToPoint(const Circle& circle, const Vector3& point);
};

