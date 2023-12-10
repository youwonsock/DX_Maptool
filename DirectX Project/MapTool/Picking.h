#pragma once

struct RAY
{
	Vector3		origin;
	Vector3		direction;
};

class Picking
{
private:
	RAY		    ray;

public:
	void UpdateRay();
	bool PickTriangle(Vector3 v0, Vector3 v1, Vector3 v2);
};

