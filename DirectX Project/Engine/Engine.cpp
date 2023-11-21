#include "pch.h"
#include "Engine.h"

#include "Global.h"

bool Engine::EngineInit()
{
	window = std::make_shared<Window>();
	graphics = std::make_shared<Graphics>();;

	graphics->Init();

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
	Update();
	PostUpdate();

	return true;
}

bool Engine::EngineRender()
{
	graphics->PreRender();
	PreRender();
	Render();
	PostRender();
	graphics->PostRender();

	return true;
}

bool Engine::EngineRelease()
{
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
