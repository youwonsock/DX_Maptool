#pragma once

#include "Graphics.h"
#include "Window.h"

class IExecute;

struct EngineDesc
{
	std::shared_ptr<IExecute> app = nullptr;
	std::wstring appName = L"GameCoding";
	HINSTANCE hInstance = 0;
	float width = 800;
	float height = 600;
	bool vsync = false;
	bool windowed = true;
};

class Engine
{
private:
	bool vSync = false;
	bool isWindowed = true;
	std::shared_ptr<IExecute> app = nullptr;

public:
	std::shared_ptr<Window> window = std::make_shared<Window>();
	std::shared_ptr<Graphics> graphics = std::make_shared<Graphics>();;

private:
	bool EngineInit();
	bool EngineFixedUpdate();
	bool EngineUpdate();
	bool EngineRender();
	bool EngineRelease();

public:
	// constructor
	Engine() {};				// use define GAME_RUN : mean use [inheritance(is-a)]
	Engine(EngineDesc& desc);	// if use this constructor : mean use [composition(has-a)]
	~Engine() {};

	// override these functions in child class
	virtual void Init()			{};

	virtual void FixedUpdate()	{};
	virtual void Update()		{};
	virtual void PostUpdate()	{};

	virtual void PreRender()	{};
	virtual void Render()		{};
	virtual void PostRender()	{};

	virtual void Release()		{};

	void Run();
};


#define GAME_RUN(str, width, height) int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)\
{\
	Sample sample;\
	sample.window->CreateClientWindow(hInstance, L#str, width, height); \
	sample.Run();\
	return 0; };\

