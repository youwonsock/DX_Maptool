#include "pch.h"
#include "Camera.h"

#include "Transform.h"

void Frustum::Update(Matrix& viewMat, Matrix& projectionMat)
{
	Matrix viewProj = viewMat * projectionMat;

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

Camera::Camera() : Base(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// set perspective range
	ImGui::InputFloat("near range", &nearRange);
	ImGui::InputFloat("far range", &farRange);

	UpdateViewMatrix();
	position = GetTransform()->GetWorldPosition();

	frustum.Update(viewMatrix, projectionMatrix);

	// if render maanger is not initialized, error!
	RenderManager::GetInstance().PushGlobalData(viewMatrix, projectionMatrix);
}

void Camera::UpdateViewMatrix()
{
	viewMatrix = GetTransform()->GetWorldMatrix().Invert();

	if (projectionType == ProjectionType::Perspective)
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), Global::g_windowWidth / Global::g_windowHeight, nearRange, farRange);
	else
		projectionMatrix = Matrix::CreateOrthographic(8, 6, 0.1f, 1000.0f);
}
