#include "pch.h"
#include "Frustum.h"

void Frustum::UpdatePlanes()
{
}

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::Update()
{
    Matrix viewProj = Camera::viewMatrix * Camera::projectionMatrix;

    //1  2   5  6
    //0  3   4  7
    frustumCorners[0] = Vector3(-1.0f, -1.0f, 0.0f);
    frustumCorners[1] = Vector3(-1.0f, 1.0f, 0.0f);
    frustumCorners[2] = Vector3(1.0f, 1.0f, 0.0f);
    frustumCorners[3] = Vector3(1.0f, -1.0f, 0.0f);

    frustumCorners[4] = Vector3(-1.0f, -1.0f, 1.0f);
    frustumCorners[5] = Vector3(-1.0f, 1.0f, 1.0f);
    frustumCorners[6] = Vector3(1.0f, 1.0f, 1.0f);
    frustumCorners[7] = Vector3(1.0f, -1.0f, 1.0f);

    for (int i = 0; i < 8; i++)
    {
        Vector3 corner = Vector3::Transform(frustumCorners[i], viewProj.Invert());
        frustumCorners[i] = corner;
	}

    //create planes
    planes[0] = Plane(frustumCorners[0], frustumCorners[1], frustumCorners[5]);
	planes[1] = Plane(frustumCorners[2], frustumCorners[3], frustumCorners[6]);
	planes[2] = Plane(frustumCorners[5], frustumCorners[2], frustumCorners[6]);
	planes[3] = Plane(frustumCorners[0], frustumCorners[7], frustumCorners[3]);
	planes[4] = Plane(frustumCorners[0], frustumCorners[2], frustumCorners[1]);
	planes[5] = Plane(frustumCorners[6], frustumCorners[4], frustumCorners[5]);



}
