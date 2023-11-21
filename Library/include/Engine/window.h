#pragma once

class Window
{
private:
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;	
	DWORD dwExStyle = 0;					
	DWORD dwWindowPosX = 100;				
	DWORD dwWindowPosY = 100;				

	std::wstring windowClassName = L"windowClass";
protected:
	HINSTANCE hInstance;					
	HWND hWnd;								
	DWORD dwWindowWidth;					
	DWORD dwWindowHeight;							
private:
	void SetRegisterClassWindow(HINSTANCE hInstance); 
	void SetWindow(const WCHAR* title, DWORD width = 800, DWORD height = 600);
public:
	virtual LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateClientWindow(HINSTANCE hInstance, const WCHAR* title, DWORD width = 800, DWORD height = 600);
public:
	Window();
	virtual ~Window() {};
};

