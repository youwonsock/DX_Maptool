#pragma once

#include "Singleton.hpp"
#include "Types.h"

enum class KeyState
{
	NONE = (0x01 << 0),   //1
	UP = (0x01 << 1),   //2
	PUSH = (0x01 << 2),   //4
	HOLD = (0x01 << 3),	  //8
};

class InputManager : public Singleton<InputManager>
{
private:
	friend class Singleton<InputManager>;
	InputManager() {};

	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pKeyboardDevice;
	IDirectInputDevice8* pMouseDevice;

	unsigned char keyboardState[256];
	KeyState keyStateArr[256];

	KeyState mouseStateArr[4];
	DIMOUSESTATE mouseState;

	POINT mousePoint;
	Vector2 mousePos;
	Vector2 beforeMousePos;
	Vector2 mouseOffset;
private:
	bool InitKeyboard(HINSTANCE hInstance, HWND hwnd);
	bool InitMouse(HINSTANCE hInstance, HWND hwnd);

	bool ReadKeyboard();
	bool ReadMouse();

	void ProcessKeyboardInput();
	void ProcessMouseInput();

public:
	bool Init(HINSTANCE hInstance, HWND hwnd);
	bool Update();
	void Release();

	Vector2  GetMousePos() const { return mousePos; };
	Vector2  GetMouseOffset() const { return mouseOffset; };
	KeyState GetKeyState(const unsigned char key) const;
	KeyState GetMouseState(const int key) const;
};

