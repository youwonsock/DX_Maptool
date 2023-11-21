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

	if(app != nullptr)
		app->Init();
	else
		Init();

	return true;
}

/// <summary>
/// not implemented	
/// </summary>
/// <returns></returns>
bool Engine::EngineFixedUpdate()
{
	return false;
}

bool Engine::EngineUpdate()
{
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
	graphics->PostRender();

	return true;
}

bool Engine::EngineRelease()
{
	if (app != nullptr)
		app->Release();
	else
		Release();

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
