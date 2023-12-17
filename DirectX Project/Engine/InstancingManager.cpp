#include "pch.h"
#include "InstancingManager.h"

#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

#include "Transform.h"


void InstancingManager::Render(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	ClearData();

	RenderMeshRenderer(gameObjects);
	RenderModelRenderer(gameObjects);
	RenderAnimRenderer(gameObjects);
}

void InstancingManager::RenderMeshRenderer(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	std::map<InstanceID, std::vector<std::shared_ptr<GameObject>>> cache;

	for (std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		const InstanceID instanceID = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceID].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const std::vector<std::shared_ptr<GameObject>>& gameObjects = pair.second;

		const InstanceID instanceID = pair.first;

		for(int i = 0; i < gameObjects.size(); ++i)
		{
			const std::shared_ptr<GameObject>& gameObj = gameObjects[i];

			InstancingData data;
			data.world = gameObj->GetTransform()->GetWorldMatrix();

			AddData(instanceID, data);
		}

		std::shared_ptr<InstancingBuffer>& buffer = buffers[instanceID];
		gameObjects[0]->GetMeshRenderer()->RenderInstancing(buffer);
	}
}

void InstancingManager::RenderModelRenderer(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	std::map<InstanceID, std::vector<std::shared_ptr<GameObject>>> cache;

	for (std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelRenderer() == nullptr)
			continue;

		const InstanceID instanceID = gameObject->GetModelRenderer()->GetInstanceID();
		cache[instanceID].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const std::vector<std::shared_ptr<GameObject>>& gameObjects = pair.second;

		const InstanceID instanceID = pair.first;

		for (int i = 0; i < gameObjects.size(); ++i)
		{
			const std::shared_ptr<GameObject>& gameObj = gameObjects[i];

			InstancingData data;
			data.world = gameObj->GetTransform()->GetWorldMatrix();

			AddData(instanceID, data);
		}

		std::shared_ptr<InstancingBuffer>& buffer = buffers[instanceID];
		gameObjects[0]->GetModelRenderer()->RenderInstancing(buffer);
	}
}

void InstancingManager::RenderAnimRenderer(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	std::map<InstanceID, std::vector<std::shared_ptr<GameObject>>> cache;

	for (std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelAnimator() == nullptr)
			continue;

		const InstanceID instanceID = gameObject->GetModelAnimator()->GetInstanceID();
		cache[instanceID].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		std::shared_ptr<InstancedTweenDesc> tweenDesc = std::make_shared<InstancedTweenDesc>();

		const std::vector<std::shared_ptr<GameObject>>& gameObjects = pair.second;
		const InstanceID instanceID = pair.first;

		for (int i = 0; i < gameObjects.size(); ++i)
		{
			const std::shared_ptr<GameObject>& gameObj = gameObjects[i];

			InstancingData data;
			data.world = gameObj->GetTransform()->GetWorldMatrix();

			AddData(instanceID, data);

			gameObj->GetModelAnimator()->UpdateTweenData();
			tweenDesc->tweens[i] = gameObj->GetModelAnimator()->GetTweenDesc();
		}

		RenderManager::GetInstance().PushInstancedTweenData(*(tweenDesc.get()));

		std::shared_ptr<InstancingBuffer>& buffer = buffers[instanceID];
		gameObjects[0]->GetModelAnimator()->RenderInstancing(buffer);
	}
}

void InstancingManager::AddData(InstanceID instanceID, InstancingData& data)
{
	if (buffers.find(instanceID) == buffers.end())
		buffers[instanceID] = std::make_shared<InstancingBuffer>();

	buffers[instanceID]->AddData(data);
}

void InstancingManager::ClearData()
{
	for(auto& buffer : buffers)
	{
		buffer.second->ClearData();
	}
}

void InstancingManager::ClearBuffer()
{
	buffers.clear();
}
