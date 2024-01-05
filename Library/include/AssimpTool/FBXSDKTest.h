#pragma once

#include "DebugDrawer.h"

class FBXSDKTest : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<GameObject> obj1;
	std::shared_ptr<GameObject> camera;

	std::shared_ptr<DebugDrawer> debugDrawer;

	void CreateKachujin();
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

