#pragma once

#include "Component.h"

class Mesh;
class Material;
class Shader;
class Texture;
class VertexShader;
class PixelShader;
class InputLayout;

class MeshRenderer : public Component
{
private:
	using Base = Component;

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Shader> shader;
public:
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetTexture(std::shared_ptr<Texture> texture);
	void SetShader(std::shared_ptr<Shader> shader);

	std::shared_ptr<Mesh> GetMesh() const;
	std::shared_ptr<Texture> GetTexture() const;
	std::shared_ptr<Shader> GetShader() const;

	virtual void Update() override;
};

