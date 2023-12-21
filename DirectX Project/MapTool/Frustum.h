#pragma once
class Frustum
{
public:
	Plane planes[6];
	Vector3 frustumCorners[8];

public:
	Frustum();
	~Frustum();

	void Update();
};

