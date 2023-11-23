#pragma once

#include "Component.h"

class Transform : public Component
{
private:
	using Base = Component;

	// local
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale = { 1,1,1 };
	Matrix localMatrix = Matrix::Identity;

	// world
	Vector3 worldPosition;
	Vector3 worldRotation;
	Vector3 worldScale = { 1,1,1 };
	Matrix worldMatrix = Matrix::Identity;

	// dirVector
	Vector3 forward;
	Vector3 right;
	Vector3 up;

private:
	std::shared_ptr<Transform> parent;
	std::vector<std::shared_ptr<Transform>> children;

public:
	Transform();
	~Transform();

	// Component을(를) 통해 상속됨
	void Init() override;
	void Update() override;

	void UpdateTransform();
	Vector3 ToEulerAngles(Quaternion q);

	// local
	Vector3 GetLocalPosition() const				{ return localPosition;}
	Vector3 GetLocalRotation() const				{ return localRotation;}
	Vector3 GetLocalScale() const					{ return localScale;}
	Matrix GetLocalMatrix()							{ return localMatrix;}
	void SetLocalPosition(const Vector3& position)  { localPosition = position; UpdateTransform(); }
	void SetLocalRotation(const Vector3& rotation)  { localRotation = rotation; UpdateTransform();}
	void SetLocalScale(const Vector3& scale)		{ localScale = scale; UpdateTransform();}

	// world
	Vector3 GetWorldPosition() const				{ return worldPosition;}
	Vector3 GetWorldRotation() const				{ return worldRotation;}
	Vector3 GetWorldScale() const					{ return worldScale;}
	Matrix GetWorldMatrix()							{ return worldMatrix;}
	void SetWorldPosition(const Vector3& position);
	void SetWorldRotation(const Vector3& rotation);
	void SetWorldScale(const Vector3& scale);

	// dirVector
	Vector3 GetForward() const						{ return forward;}
	Vector3 GetRight() const						{ return right;}
	Vector3 GetUp() const							{ return up;}
	
	// 계층 관계
	bool HasParent() const							  { return parent != nullptr;}

	void SetParent(std::shared_ptr<Transform> parent) { this->parent = parent; }
	std::shared_ptr<Transform> GetParent() const	{return parent;}

	const std::vector<std::shared_ptr<Transform>> GetChildren() { return children; }
	void AddChild(std::shared_ptr<Transform> child) { children.push_back(child); }
};

