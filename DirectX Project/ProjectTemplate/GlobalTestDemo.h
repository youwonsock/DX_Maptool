#pragma once

class GlobalTestDemo : public IExecute
{
private:
	std::shared_ptr<GameObject> obj;
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

