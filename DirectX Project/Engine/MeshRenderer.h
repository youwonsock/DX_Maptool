#pragma once

#include "Component.h"
#include "Types.h"

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
	UINT pass = 0;
public:
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetPass(UINT pass);

	std::shared_ptr<Mesh> GetMesh() const;
	std::shared_ptr<Material> GetMaterial() const;
	UINT GetPass() const;

	InstanceID GetInstanceID() const;
	void RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer);
};

