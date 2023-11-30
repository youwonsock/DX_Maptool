#include "pch.h"
#include "WinMain.h"

#include "StaticMeshDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	EngineDesc gameDesc;
	gameDesc.appName = L"has-a engine";
	gameDesc.hInstance = hInstance;
	gameDesc.vsync = false;
	gameDesc.width = 800;
	gameDesc.height = 600;
	gameDesc.app = std::make_shared<StaticMeshDemo>();

	Engine game(gameDesc);

	return 0;
};