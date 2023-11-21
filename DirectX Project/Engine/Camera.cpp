#include "pch.h"
#include "Camera.h"

#include "Transform.h"

Matrix Camera::viewMatrix = Matrix::Identity;
Matrix Camera::projectionMatrix = Matrix::Identity;

Camera::Camera() : Base(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	viewMatrix = GetTransform()->GetWorldMatrix().Invert();

	if (projectionType == ProjectionType::Perspective)
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), Global::g_windowWidth / Global::g_windowHeight, 0.1f, 1000.0f);
	else
		projectionMatrix = Matrix::CreateOrthographic(8, 6, 0.1f, 1000.0f);
}
