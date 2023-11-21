#include "pch.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"

Material::Material() : Base(ResourceType::Material)
{
}

Material::~Material()
{
}

void Material::SetShader(const std::shared_ptr<Shader>& shader)
{
	this->shader = shader;
}

std::shared_ptr<Shader> Material::GetShader() const
{
	return shader;
}

void Material::SetTexture(const std::shared_ptr<Texture>& texture)
{
	this->texture = texture;
}

std::shared_ptr<Texture> Material::GetTexture() const
{
	return texture;
}
