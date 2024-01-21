#pragma once

class SpaceDivideTree;
class ObjectManager;

#include "DepthMapShadow.h"

class MapRenderer
{
private:
	std::shared_ptr<Shader> terrainShader;
	std::shared_ptr<Shader> objectShader;

	Matrix view;
	Matrix proj;

	LightDesc lightDesc;


	// sky box
	std::shared_ptr<Shader> skyboxShader;
	std::shared_ptr<Texture> cubemapTexture;
	ComPtr<ID3DX11EffectShaderResourceVariable> cubemapSRV;
	TransformDesc transformDesc;

	std::shared_ptr<Mesh> box;

	// ȯ�� ����
	std::shared_ptr<Mesh> mappingMesh;

	// �׸���
	DepthMapShadow depthMapShadow;

public:
	void Update();
	void Init();
	void Render();

	void RenderShadow();

	MapRenderer();
	~MapRenderer();
};

