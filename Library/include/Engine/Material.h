#pragma once

#include "ResourceBase.h"

class Shader;
class Texture;

class Material : public ResourceBase
{
private:
	using Base = ResourceBase;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;

public:
	Material();
	~Material();

	void SetShader(const std::shared_ptr<Shader>& shader);
	std::shared_ptr<Shader> GetShader() const;
	
	void SetTexture(const std::shared_ptr<Texture>& texture);
	std::shared_ptr<Texture> GetTexture() const;
};

