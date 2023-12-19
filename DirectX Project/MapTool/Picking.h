#pragma once

class Picking
{
private:
	Ray		    ray;

public:
	void UpdateRay();

	Ray& GetRay() { return ray; }
};

