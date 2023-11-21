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
	std::shared_ptr<Material> material;
public:
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh> mesh);
	std::shared_ptr<Mesh> GetMesh() const;

	void SetMaterial(std::shared_ptr<Material> material);
	void SetShader(std::shared_ptr<Shader> shader);
	std::shared_ptr<Material> GetMeterial() const;
	std::shared_ptr<VertexShader> GetVertexShader() const;
	std::shared_ptr<PixelShader> GetPixelShader() const;
	std::shared_ptr<InputLayout> GetInputLayout() const;

	void SetTexture(std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> GetTexture() const;
};

