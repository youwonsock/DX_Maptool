#pragma once
class MapRenderer
{
private:
	std::shared_ptr<Shader> terrainShader;
	std::shared_ptr<Shader> objectShader;

	Matrix view;
	Matrix proj;

	LightDesc lightDesc;


	std::shared_ptr<Shader> skyboxShader;
	std::shared_ptr<Texture> cubemapTexture;
	TransformDesc transformDesc;
public:
	void Update();
	void Init();

	MapRenderer();
	~MapRenderer();
};

