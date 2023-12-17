#pragma once

class Picking
{
private:
	Ray		    ray;

public:
	void UpdateRay();
	bool PickTriangle(Vector3 v0, Vector3 v1, Vector3 v2);

	Ray& GetRay() { return ray; }
};

