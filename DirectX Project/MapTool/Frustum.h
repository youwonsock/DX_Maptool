#pragma once
class Frustum
{
public:
	Plane planes[6];
	Vector3 frustumCorners[8];

private:
	void UpdatePlanes();

public:
	Frustum();
	~Frustum();

	void Update();
};

