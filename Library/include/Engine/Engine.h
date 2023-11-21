#pragma once

#include "Graphics.h"
#include "Window.h"

class Engine
{

private:
	bool EngineInit();
	bool EngineFixedUpdate();
	bool EngineUpdate();
	bool EngineRender();
	bool EngineRelease();
	
public:
	std::shared_ptr<Window> window = std::make_shared<Window>();
	std::shared_ptr<Graphics> graphics = std::make_shared<Graphics>();;

public:
	// constructor
	Engine() {};
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

