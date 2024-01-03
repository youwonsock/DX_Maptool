#include "pch.h"
#include "ObjectManager.h"

#include "SectionNode.h"

void ObjectManager::Init(std::wstring sceneFilePath, std::wstring shaderFilePath)
{
	if(scene != nullptr)
		scene.reset();
	scene = std::make_shared<Scene>();

	if (sceneFilePath.size() > 1)
		scene->LoadScene(sceneFilePath);

	SceneManager::GetInstance().ChangeScene(scene);

	// shader
	shader = std::make_shared<Shader>(L"MapToolShader/ObjectShader.fx");

	RenderManager::GetInstance().Init(shader);

	ResourceManager::GetInstance().Load<Model>(L"Tower", L"Tower/Tower", false);
}

void ObjectManager::SpawnObject(const std::wstring& objectModelName, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap)
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));

	auto model = ResourceManager::GetInstance().Get<Model>(objectModelName);
	obj->GetModelRenderer()->SetModel(model);
	int pass = model->HasAnimation() ? 1 : 0;
	obj->GetModelRenderer()->SetPass(pass);

	obj->GetTransform()->SetWorldPosition(spawnPoint);

	// set obj group node
	for (auto& leafNode : leafNodeMap)
	{
		if (Collision::CubeToCube(leafNode.second->boundingBox , obj->GetTransform()->GetBoundingBox()))
		{
			obj->groupNodeIdxList.push_back(leafNode.first);
			SceneManager::GetInstance().GetCurrentScene()->Add(obj, leafNode.first);
		}
	}
}

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{

}
