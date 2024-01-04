#include "pch.h"
#include "TestCamera.h"

#include "Transform.h"

void TestCamera::BeginPlay()
{

}

void TestCamera::Update()
{
	Camera::Update();

	ray.UpdateRay(viewMatrix, projectionMatrix);

	double deltaTime = TimeManager::GetInstance().GetDeltaTime();

	Vector3 pos = GetTransform()->GetLocalPosition();
	Vector3 rotation = GetTransform()->GetLocalRotation();

	if (InputManager::GetInstance().GetKeyState(DIK_W) == KeyState::HOLD)
		pos += GetTransform()->GetForward() * speed * deltaTime;

	if (InputManager::GetInstance().GetKeyState(DIK_S) == KeyState::HOLD)
		pos -= GetTransform()->GetForward() * speed * deltaTime;

	if (InputManager::GetInstance().GetKeyState(DIK_A) == KeyState::HOLD)
		pos -= GetTransform()->GetRight() * speed * deltaTime;

	if (InputManager::GetInstance().GetKeyState(DIK_D) == KeyState::HOLD)
		pos += GetTransform()->GetRight() * speed * deltaTime;

	if (InputManager::GetInstance().GetKeyState(DIK_Q) > KeyState::UP)
		pos += GetTransform()->GetUp() * speed * deltaTime;

	if (InputManager::GetInstance().GetKeyState(DIK_E) > KeyState::UP)
		pos -= GetTransform()->GetUp() * speed * deltaTime;

	if (InputManager::GetInstance().GetMouseState(1) > KeyState::UP)
	{
		Vector2 offset = InputManager::GetInstance().GetMouseOffset();

		rotation.y += offset.x * deltaTime * rotationSpeed;
		rotation.x += offset.y * deltaTime * rotationSpeed;
	}

	GetTransform()->SetLocalPosition(pos);
	GetTransform()->SetLocalRotation(rotation);
}

void TestCamera::PostUpdate()
{
	ImGui::Begin("Camera", nullptr, window_flags);
	{
		ImGui::SetWindowSize("Camera", ImVec2(350, 125));
		ImGui::SetWindowPos("Camera", ImVec2(Global::g_windowWidth - 350, Global::g_windowHeight - 125));

		ImGui::InputFloat("near range", &nearRange);
		ImGui::InputFloat("far range", &farRange);
		ImGui::InputFloat("cam move speed", &speed);
		ImGui::InputFloat("cam rotation speed", &rotationSpeed);
	}
	ImGui::End();
}
