#pragma once

#include "TestCamera.h"
#include "HeightMap.h"

class MapTool : public IExecute
{
private:
	std::shared_ptr<Shader> shader;

	std::shared_ptr<HeightMap> heightMapComponent;
	std::shared_ptr<GameObject> map;
	std::shared_ptr<GameObject> cameraObject;

public:
	// IExecute을(를) 통해 상속됨
	void Init() override;
	void FixedUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Release() override;
};

