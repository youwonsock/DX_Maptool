#pragma once

class MapTool : public IExecute
{
private:
	std::shared_ptr<GameObject> map;

	std::shared_ptr<GameObject> cameraObject;

public:
	// IExecute��(��) ���� ��ӵ�
	void Init() override;
	void FixedUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Release() override;
};

