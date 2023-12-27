#include "pch.h"
#include "WinMain.h"

#include "AssimpTool.h"
#include "FBXSDKTest.h"

#include "SkyDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	EngineDesc gameDesc;
	gameDesc.appName = L"has-a engine";
	gameDesc.hInstance = hInstance;
	gameDesc.vsync = false;
	gameDesc.width = 800;
	gameDesc.height = 600;

	gameDesc.app = std::make_shared<FBXSDKTest>();
	//gameDesc.app = std::make_shared<SkyDemo>();

	Engine game(gameDesc);

	return 0;
};