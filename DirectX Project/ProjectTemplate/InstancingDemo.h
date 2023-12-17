#pragma once

class InstancingDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<GameObject>> objs;
	std::shared_ptr<GameObject> cameraObject;

private:

	// instancing
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;

	std::vector<Matrix> worldMats;
	std::shared_ptr<VertexBuffer> instanceBuffer;
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

