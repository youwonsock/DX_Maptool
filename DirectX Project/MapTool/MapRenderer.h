#pragma once
class MapRenderer
{
private:
	std::shared_ptr<Shader> terrainShader;
	std::shared_ptr<Shader> objectShader;

	Matrix view;
	Matrix proj;

public:
	void Update();
	void Init();

	MapRenderer();
	~MapRenderer();
};

