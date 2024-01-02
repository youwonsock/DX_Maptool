#include "pch.h"
#include "ObjectManager.h"

#include "SectionNode.h"

void ObjectManager::Init(std::wstring sceneFilePath, std::wstring shaderFilePath)
{
	this->sceneFilePath = sceneFilePath;

	if(scene != nullptr)
		scene.reset();
	scene = std::make_shared<Scene>();

	if (sceneFilePath.size() > 1)
		scene->LoadScene(sceneFilePath);

	shader = std::make_shared<Shader>(L"MapToolShader/ObjectShader.fx");

	SceneManager::GetInstance().ChangeScene(scene);
}

void ObjectManager::SpawnObject(std::wstring& objectModelPath, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	
	// objectModelPath = L"Tower/Tower";
	model->ReadModel(L"Tower/Tower");
	model->ReadMaterial(L"Tower/Tower");
	model->ReadAnimation(L"Tower/Tower");

	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();

	obj->AddComponent(std::make_shared<ModelRenderer>(shader));
	obj->GetModelRenderer()->SetModel(model);

	int pass = model->HasAnimation() ? 1 : 0;
	obj->GetModelRenderer()->SetPass(pass);

	obj->GetTransform()->SetWorldPosition(spawnPoint);

	// set obj group node
	for (auto& leafNode : leafNodeMap)
	{
		if (Collision::CubeToCube(leafNode.second->boundingBox(), obj->GetTransform()->GetBoundingBox()))
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
