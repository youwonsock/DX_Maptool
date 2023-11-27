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

	diffuseEffectBuffer = shader->GetSRV("DiffuseMap");
	normalEffectBuffer = shader->GetSRV("NormalMap");
	specularEffectBuffer = shader->GetSRV("SpecularMap");
}

void Material::SetDiffuseMap(const std::shared_ptr<Texture>& diffuseMap)
{
	this->diffuseMap = diffuseMap;
}

void Material::SetNormalMap(const std::shared_ptr<Texture>& normalMap)
{
	this->normalMap = normalMap;
}

void Material::SetSpecularMap(const std::shared_ptr<Texture>& specularMap)
{
	this->specularMap = specularMap;
}

const std::shared_ptr<Shader>& Material::GetShader() const
{
	return shader;
}

const std::shared_ptr<Texture>& Material::GetDiffuseMap() const
{
	return diffuseMap;
}

const std::shared_ptr<Texture>& Material::GetNormalMap() const
{
	return normalMap;
}

const std::shared_ptr<Texture>& Material::GetSpecularMap() const
{
	return specularMap;
}

const MaterialDesc& Material::GetMaterialDesc() const
{
	return desc;
}

void Material::Update()
{
	if (shader == nullptr)
		return;

	RenderManager::GetInstance().PushMaterialData(desc);

	if (diffuseMap != nullptr)
		diffuseEffectBuffer->SetResource(diffuseMap->GetShaderResourceView().Get());

	if (normalMap != nullptr)
		normalEffectBuffer->SetResource(normalMap->GetShaderResourceView().Get());

	if (specularMap != nullptr)
		specularEffectBuffer->SetResource(specularMap->GetShaderResourceView().Get());
}

std::shared_ptr<Material> Material::Clone() const
{
	return std::shared_ptr<Material>();
}
