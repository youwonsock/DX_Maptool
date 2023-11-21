#pragma once

#include "ResourceBase.h"

class ShaderRes;
class Texture;

class Material : public ResourceBase
{
private:
	using Base = ResourceBase;

	std::shared_ptr<ShaderRes> shader;
	std::shared_ptr<Texture> texture;

public:
	Material();
	~Material();

	void SetShader(const std::shared_ptr<ShaderRes>& shader);
	std::shared_ptr<ShaderRes> GetShader() const;
	
	void SetTexture(const std::shared_ptr<Texture>& texture);
	std::shared_ptr<Texture> GetTexture() const;
};

