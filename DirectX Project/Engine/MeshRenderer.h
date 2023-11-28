#pragma once

#include "Component.h"

class Mesh;
class Shader;
class Texture;
class Material;

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
	void SetMaterial(std::shared_ptr<Material> material);

	std::shared_ptr<Mesh> GetMesh() const;
	std::shared_ptr<Material> GetMaterial() const;

	virtual void Update() override;
};

