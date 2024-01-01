#pragma once

#include "Singleton.hpp"
#include "Types.h" 

class InstancingBuffer;
class GameObject;

struct InstancingData;

class InstancingManager : public Singleton<InstancingManager>
{
private:
	friend class Singleton<InstancingManager>;
	InstancingManager() {};

	std::map<InstanceID, std::shared_ptr<InstancingBuffer>> buffers;
public:


private:
	void RenderMeshRenderer(std::set<std::shared_ptr<GameObject>>& gameObjects);
	void RenderModelRenderer(std::set<std::shared_ptr<GameObject>>& gameObjects);
	void RenderAnimRenderer(std::set<std::shared_ptr<GameObject>>& gameObjects);

	void AddData(InstanceID instanceID, InstancingData& data);

public:
	void Render(std::set<std::shared_ptr<GameObject>>& gameObjects);
	void ClearData();
	void ClearBuffer();
};

