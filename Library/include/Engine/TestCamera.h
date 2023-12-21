#pragma once

#include "Camera.h"

class TestCamera : public Camera
{
private:
	float speed = 100.0f;
	float rotationSpeed = 5.0f;
public:
	TestCamera() : Camera() {};
	virtual ~TestCamera() {};

	virtual void BeginPlay()	override;
	virtual void Update()		override;
};

