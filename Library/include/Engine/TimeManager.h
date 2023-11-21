#pragma once

#include "Singleton.hpp"

using Seconds = std::chrono::microseconds;
const double TIME_EPSILON = 0.000001f;

class TimeManager : public Singleton<TimeManager>
{
private:
	friend class Singleton<TimeManager>;
	TimeManager() {};

	double deltaTime;
	double gameTime;
	std::chrono::high_resolution_clock::time_point lastTime;
public:
	void Init();
	void Update();
	void Render();
	void Release();

	double GetDeltaTime() const { return deltaTime; }
	double GetGameTime() const { return gameTime; }
	int GetFPS() const { return (int)(1.0f / deltaTime); }
};

