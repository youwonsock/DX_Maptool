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
	ComPtr<ID3DX11EffectShaderResourceVariable> cubemapSRV;
	TransformDesc transformDesc;

	std::shared_ptr<Mesh> box;

public:
	void Update();
	void Init();
	void Render();

	MapRenderer();
	~MapRenderer();
};

