#include "pch.h"
#include "TestCamera.h"

void TestCamera::BeginPlay()
{

}

void TestCamera::Update()
{
	double deltaTime = TimeManager::GetInstance().GetDeltaTime();

	Vector3 pos = GetTransform()->GetLocalPosition();

	if (InputManager::GetInstance().GetKeyState(DIK_W) == KeyState::HOLD)
	{
		pos += GetTransform()->GetForward() * speed * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_S) == KeyState::HOLD)
	{
		pos -= GetTransform()->GetForward() * speed * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_A) == KeyState::HOLD)
	{
		pos -= GetTransform()->GetRight() * speed * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_D) == KeyState::HOLD)
	{
		pos += GetTransform()->GetRight() * speed * deltaTime;
	}

	Vector3 rotation = GetTransform()->GetLocalRotation();
	if (InputManager::GetInstance().GetKeyState(DIK_Q) == KeyState::HOLD)
	{
		rotation.x += deltaTime * 0.5f;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_E) == KeyState::HOLD)
	{
		rotation.x -= deltaTime * 0.5f;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_Z) == KeyState::HOLD)
	{
		rotation.y += deltaTime * 0.5f;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_C) == KeyState::HOLD)
	{
		rotation.y -= deltaTime * 0.5f;
	}

	GetTransform()->SetLocalPosition(pos);
	GetTransform()->SetLocalRotation(rotation);
}
