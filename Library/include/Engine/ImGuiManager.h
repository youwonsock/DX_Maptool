#pragma once

#include "Singleton.hpp"

class ImGuiManager : public Singleton<ImGuiManager>
{
private:
	friend class Singleton<ImGuiManager>;
	ImGuiManager() {};

public:
	void Init();
	void Update();
	void Render();

	void Release();

};

