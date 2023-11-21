#include "pch.h"
#include "SceneManager.h"

#include "ResourceHeader.h"
#include "EngineHeader.h"

#include "Global.h"

void SceneManager::Init()
{
	if (currentScene != nullptr)
	{
		currentScene->Init();
		currentScene->BeginPlay();
	}
}

void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->FixedUpdate();
		currentScene->Update();
		currentScene->PostUpdate();
	}
}

void SceneManager::LoadScene(std::wstring sceneName)
{
	// make test scene

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();

	// Camera
	{
		std::shared_ptr<GameObject> camera = std::make_shared<GameObject>();
		{
			camera->GetTransform();
			camera->AddComponent(std::make_shared<Camera>());
			scene->AddGameObject(camera);
		}
	}

	// Monster
	{
		std::shared_ptr<GameObject> monster = std::make_shared<GameObject>();
		monster->GetTransform()->SetWorldPosition(Vector3{ 1.f, 1.f, 0.f });
		{
			monster->GetTransform();
			auto meshRenderer = std::make_shared<MeshRenderer>();
			monster->AddComponent(meshRenderer);

			auto material = ResourceManager::GetInstance().Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = std::make_shared<Animator>();
			monster->AddComponent(animator);
			auto anim = ResourceManager::GetInstance().Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(monster);
	}

	// Monster
	{
		std::shared_ptr<GameObject> monster = std::make_shared<GameObject>();
		{
			monster->GetTransform();
			auto meshRenderer = std::make_shared<MeshRenderer>();
			monster->AddComponent(meshRenderer);
			auto material = ResourceManager::GetInstance().Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = std::make_shared<Animator>();
			monster->AddComponent(animator);
			auto anim = ResourceManager::GetInstance().Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(monster);
	}
	// make test scene


	currentScene = scene;
}

const std::shared_ptr<Scene> SceneManager::GetCurrentScene() const
{
	return currentScene;
}
