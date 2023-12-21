#pragma once

#include "InputManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ImGuiManager.h"	
#include "InstancingManager.h"
#include "SceneManager.h"
#include "CameraManager.h"

using namespace Microsoft::WRL;					// for ComPtr

class Global
{
public:
	static int g_windowWidth;
	static int g_windowHeight;

	static HWND g_hWnd;
	static HINSTANCE g_hInstance;
	static ComPtr<ID3D11Device> g_device;
	static ComPtr<ID3D11DeviceContext> g_immediateContext;
};