#pragma once

class StructredBufferDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
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

