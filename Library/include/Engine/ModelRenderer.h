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
	UINT8					pass = 0;
	std::shared_ptr<Model>	model;

public: 
	ModelRenderer(std::shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	virtual void Update() override;

	void SetModel(std::shared_ptr<Model> model);
	void SetPass(UINT8 pass) { this->pass = pass; }
};

