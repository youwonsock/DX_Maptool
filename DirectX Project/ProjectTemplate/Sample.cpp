#include "Sample.h"

#include "Engine/Global.h"

void Sample::Init()
{
	// init Manager class
	InputManager::GetInstance().Init(Global::g_hInstance, Global::g_hWnd);
	TimeManager::GetInstance().Init();
	SceneManager::GetInstance().Init();
	ResourceManager::GetInstance().Init();
	RenderManager::GetInstance().Init();

	// init Scene
	SceneManager::GetInstance().LoadScene(L"Scene1");
}

void Sample::FixedUpdate()
{
}

void Sample::Update()
{
	TimeManager::GetInstance().Update();
	InputManager::GetInstance().Update();
	SceneManager::GetInstance().Update();
}

void Sample::PostUpdate()
{
}

void Sample::PreRender()
{
}

void Sample::Render()
{
	RenderManager::GetInstance().Render();
}

void Sample::PostRender()
{
}

void Sample::Release()
{
}

GAME_RUN(Test, 800, 600);