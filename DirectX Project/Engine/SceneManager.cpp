#include "pch.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	currentScene->Init();
}

void SceneManager::BeginPlay()
{
	currentScene->BeginPlay();
}

void SceneManager::FixedUpdate()
{
	currentScene->FixedUpdate();
}

void SceneManager::Update()
{
	currentScene->Update();
}

void SceneManager::PostUpdate()
{
	currentScene->PostUpdate();
}

void SceneManager::PreRender()
{
	currentScene->PreRender();
}

void SceneManager::Render()
{
	currentScene->Render();
}

void SceneManager::PostRender()
{
	currentScene->PostRender();
}

void SceneManager::Release()
{
	currentScene->Release();
}
