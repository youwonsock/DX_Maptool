#pragma once
#include "Component.h"

class Model;
class Shader;
class Material;

class ModelRenderer : public Component
{
private:
	using Base = Component;

	std::shared_ptr<Shader>	shader;
	std::shared_ptr<Model>	model;
	UINT8					pass = 0;

public: 
	ModelRenderer(std::shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	void SetModel(std::shared_ptr<Model> model);
	void SetPass(UINT8 pass) { this->pass = pass; }

	InstanceID GetInstanceID() const;
	void RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer);
	virtual void Render() override;
};

