#include "pch.h"
#include "Engine.h"

#include "Global.h"
#include "IExecute.h"

Engine::Engine(EngineDesc& desc)
{
	window->CreateClientWindow(desc.hInstance, desc.appName.c_str(), desc.width, desc.height);

	vSync = desc.vsync;
	isWindowed = desc.windowed;
	app = desc.app;

	Run();
}

bool Engine::EngineInit()
{
	window = std::make_shared<Window>();
	graphics = std::make_shared<Graphics>();;

	graphics->Init();

	// Init Manager
	{
		TimeManager::GetInstance().Init();
		InputManager::GetInstance().Init(Global::g_hInstance, Global::g_hWnd);
		ResourceManager::GetInstance().Init();
		ImGuiManager::GetInstance().Init();
	}

	if (app != nullptr)
	{
		app->Init();
	}
	else
	{
		Init();
	}

	{
		SceneManager::GetInstance().Init();
		SceneManager::GetInstance().BeginPlay();

		CameraManager::GetInstance().Init();
	}

	return true;
}

/// <summary>
/// not implemented	
/// </summary>
/// <returns></returns>
bool Engine::EngineFixedUpdate()
{
	if (app != nullptr)
	{
		app->FixedUpdate();
	}
	else
	{
		FixedUpdate();
	}

	{
		CameraManager::GetInstance().FixedUpdate();
		SceneManager::GetInstance().FixedUpdate();
	}

	return false;
}

bool Engine::EngineUpdate()
{
	// update manager
	{
		TimeManager::GetInstance().Update();
		InputManager::GetInstance().Update();
		ImGuiManager::GetInstance().Update();
	}

	if (app != nullptr)
	{
		app->Update();
		app->PostUpdate();
	}
	else
	{
		Update();
		PostUpdate();
	}

	{
		CameraManager::GetInstance().Update();
		SceneManager::GetInstance().Update();

		CameraManager::GetInstance().PostUpdate();
		SceneManager::GetInstance().PostUpdate();
	}

	return true;
}

bool Engine::EngineRender()
{
	graphics->PreRender();

	if (app != nullptr)
	{
		app->PreRender();
		app->Render();
		app->PostRender();
	}
	else
	{
		PreRender();
		Render();
		PostRender();
	}

	{
		SceneManager::GetInstance().PreRender();
		SceneManager::GetInstance().Render();
		SceneManager::GetInstance().PostRender();
	}

	ImGuiManager::GetInstance().Render();
	graphics->PostRender();

	return true;
}

bool Engine::EngineRelease()
{
	if (app != nullptr)
		app->Release();
	else
		Release();

	CameraManager::GetInstance().Release();
	SceneManager::GetInstance().Release();

	graphics->Release();

	return true;
}

void Engine::Run()
{
	EngineInit();

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!EngineUpdate() || !EngineRender())
				break;
		}
	}

	EngineRelease();
}
