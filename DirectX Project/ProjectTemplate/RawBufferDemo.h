#pragma once

class RawBufferDemo : public IExecute
{
	struct Output // output�����͸� ���ϰ� �޾ƿ��� ���� ������ ����ü
	{
		UINT groupID[3];
		UINT groupThreadID[3];
		UINT dispatchThreadID[3];
		UINT groupIndex;
	};


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

