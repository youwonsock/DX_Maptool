#pragma once

class StaticMeshDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<GameObject> obj;
	std::shared_ptr<GameObject> camera;

	void CreateTower();
	void CreateTank();

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

