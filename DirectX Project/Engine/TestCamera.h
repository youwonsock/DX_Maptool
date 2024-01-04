#pragma once

#include "Camera.h"

class TestCamera : public Camera
{
private:
	float speed = 100.0f;
	float rotationSpeed = 5.0f;

	ImGuiWindowFlags window_flags = 0;

public:
	TestCamera() : Camera() 
	{
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;
	};
	virtual ~TestCamera() {};

	virtual void BeginPlay()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
};

