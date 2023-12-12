#pragma once
class SkyBox : public MonoBehaviour
{
private:
	std::unique_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;

	std::shared_ptr<Mesh> skyMesh;

public:
	void Render() override;

	SkyBox();	
	~SkyBox();
};

