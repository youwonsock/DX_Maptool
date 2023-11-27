#pragma once

#include "ResourceBase.h"

class Shader;
class Texture;

class Material : public ResourceBase
{
private:
	using Base = ResourceBase;

	MaterialDesc desc;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Texture> specularMap;

	ComPtr<ID3DX11EffectShaderResourceVariable> diffuseEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> normalEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> specularEffectBuffer;
public:
	Material();
	virtual ~Material();

	void SetShader(const std::shared_ptr<Shader>& shader);
	void SetDiffuseMap(const std::shared_ptr<Texture>& diffuseMap);
	void SetNormalMap(const std::shared_ptr<Texture>& normalMap);
	void SetSpecularMap(const std::shared_ptr<Texture>& specularMap);

	const std::shared_ptr<Shader>& GetShader() const;
	const std::shared_ptr<Texture>& GetDiffuseMap() const;
	const std::shared_ptr<Texture>& GetNormalMap() const;
	const std::shared_ptr<Texture>& GetSpecularMap() const;
	MaterialDesc& GetMaterialDesc();

	void Update();
	
	std::shared_ptr<Material> Clone() const;
};

