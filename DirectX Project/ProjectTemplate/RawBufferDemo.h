#pragma once

class RawBufferDemo : public IExecute
{
	struct Output // output데이터를 편하게 받아오기 위해 정의한 구조체
	{
		UINT groupID[3];
		UINT groupThreadID[3];
		UINT dispatchThreadID[3];
		UINT groupIndex;
	};


private:
	std::shared_ptr<Shader> shader;
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

