#include "pch.h"
#include "Window.h"

#include "StdHeader.h"

Window* pWindow = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	LRESULT hr = pWindow->MessageHandler(hWnd, message, wParam, lParam);
	if (hr > 1)
		return hr;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // return message to window
	}

	return 0;
}

LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (SIZE_MINIMIZED == wParam)
			break;

		// do something

		break;
	}

	return -1;
}

void Window::SetRegisterClassWindow(HINSTANCE hInstance)
{
	this->hInstance = hInstance;				

	WNDCLASSEX wcex;							
	ZeroMemory(&wcex, sizeof(wcex));			

	//set window class EX members value
	wcex.cbSize = sizeof(WNDCLASSEX);			
	wcex.style = CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc = WndProc;					
	wcex.hInstance = hInstance;					
	wcex.lpszClassName = windowClassName.c_str();
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));	
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);					

	WORD ret = RegisterClassEx(&wcex);								

	if (!ret)
		assert(false); 
}

void Window::SetWindow(const WCHAR* title, DWORD width, DWORD height)
{
	dwWindowHeight = Global::g_windowHeight = height;
	dwWindowWidth = Global::g_windowWidth = width;

#ifdef _DEBUG
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle |= WS_VISIBLE;
#else // release
	dwStyle = WS_POPUPWINDOW;
	dwExStyle = WS_EX_TOPMOST;
#endif
	RECT rc = { 0, 0, dwWindowWidth, dwWindowHeight };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle); // adjust window size

	// create window
	// window class name is need to same with window class EX name
	hWnd = CreateWindowEx(dwExStyle, windowClassName.c_str(), title, dwStyle
		, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top
		, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		assert(false); // failed to create window

	ShowWindow(hWnd, SW_SHOWNORMAL);	// show window
	Global::g_hWnd = hWnd;
}

void Window::CreateClientWindow(HINSTANCE hInstance, const WCHAR* title, DWORD width, DWORD height)
{
	SetRegisterClassWindow(hInstance);	// set window class
	SetWindow(title, width, height);	// set window
	Global::g_hInstance = hInstance;
}

Window::Window()
{
	pWindow = this;
}
