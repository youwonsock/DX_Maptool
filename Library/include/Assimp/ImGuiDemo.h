#pragma once

class ImGuiDemo : public IExecute
{
private:
	bool show_demo_window = true;
	bool show_another_window = false;
	Vector4 clear_color = Vector4(0.45f, 0.55f, 0.60f, 1.00f);

	void Test();
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

