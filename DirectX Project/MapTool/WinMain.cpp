#include "pch.h"
#include "WinMain.h"

#include "MapTool.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	EngineDesc gameDesc;
	gameDesc.appName = L"MapTool";
	gameDesc.hInstance = hInstance;
	gameDesc.vsync = false;
	gameDesc.width = 1280;
	gameDesc.height = 720;

	gameDesc.app = std::make_shared<MapTool>();

	Engine game(gameDesc);

	_CrtDumpMemoryLeaks();

	return 0;
};