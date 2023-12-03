#pragma once

#include "Component.h"

class Model;
class Shader;

class ModelAnimator : public Component
{
private:
	using Base = Component;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Model> model;
	UINT pass = 0;

public:
	ModelAnimator(std::shared_ptr<Shader> shader);
	virtual ~ModelAnimator();
	
	virtual void Update() override;

	void SetModel(std::shared_ptr<Model> model);
	void SetPass(UINT pass) { this->pass = pass; }
};

