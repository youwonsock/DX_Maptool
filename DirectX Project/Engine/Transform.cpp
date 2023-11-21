#include "pch.h"
#include "Transform.h"

Transform::Transform() : Base(ComponentType::Transform)
{
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
}


Vector3 Transform::ToEulerAngles(Quaternion q)
{
	Vector3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	Matrix matScale = Matrix::CreateScale(localScale);
	Matrix matRotation = Matrix::CreateRotationX(localRotation.x);
	matRotation *= Matrix::CreateRotationY(localRotation.y);
	matRotation *= Matrix::CreateRotationZ(localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(localPosition);

	localMatrix = matScale * matRotation * matTranslation;

	if (HasParent())
	{
		worldMatrix = localMatrix * parent->GetWorldMatrix();
	}
	else
	{
		worldMatrix = localMatrix;
	}

	Quaternion quat;
	worldMatrix.Decompose(worldScale, quat, worldPosition);
	worldRotation = ToEulerAngles(quat);

	right = Vector3::TransformNormal(Vector3::Right, worldMatrix);
	up = Vector3::TransformNormal(Vector3::Up, worldMatrix);
	forward = Vector3::TransformNormal(Vector3::Backward, worldMatrix); // simple math lib is right handed

	for (const std::shared_ptr<Transform>& child : children)
		child->UpdateTransform();
}

void Transform::SetWorldPosition(const Vector3& position)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = parent->GetWorldMatrix().Invert();

		Vector3 localPosition;
		localPosition.Transform(position, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
		SetLocalPosition(position);
}

void Transform::SetWorldRotation(const Vector3& rotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = parent->GetWorldMatrix().Invert();

		Vector3 localRotation;
		localRotation.TransformNormal(rotation, inverseMatrix);

		SetLocalRotation(localRotation);
	}
	else
		SetLocalRotation(rotation);
}

void Transform::SetWorldScale(const Vector3& scale)
{
	if (HasParent())
	{
		Vector3 parentScale = parent->GetWorldScale();
		Vector3 localScale = scale;
		localScale.x /= parentScale.x;
		localScale.y /= parentScale.y;
		localScale.z /= parentScale.z;
		SetLocalScale(localScale);
	}
	else
		SetLocalScale(scale);
}
