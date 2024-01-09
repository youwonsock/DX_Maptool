#include "pch.h"
#include "WinMain.h"

#include "TextureBufferDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	EngineDesc gameDesc;
	gameDesc.appName = L"has-a engine";
	gameDesc.hInstance = hInstance;
	gameDesc.vsync = false;
	gameDesc.width = 800;
	gameDesc.height = 600;
	gameDesc.app = std::make_shared<TextureBufferDemo>();

	Engine game(gameDesc);

	return 0;
};