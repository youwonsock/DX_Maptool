#include "pch.h"
#include "Global.h"

int Global::g_windowWidth = 0;
int Global::g_windowHeight = 0;

HWND Global::g_hWnd = nullptr;
HINSTANCE Global::g_hInstance = nullptr;
ComPtr<ID3D11Device> Global::g_device = nullptr;
ComPtr<ID3D11DeviceContext> Global::g_immediateContext = nullptr;