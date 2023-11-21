#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	deltaTime = 0;
	gameTime = 0;
	lastTime = std::chrono::high_resolution_clock::now();
}
void TimeManager::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::duration_cast<Seconds>(currentTime - lastTime).count() * TIME_EPSILON;

	lastTime = currentTime;
	gameTime += deltaTime;
}
void TimeManager::Render()
{
}
void TimeManager::Release()
{

}