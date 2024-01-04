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

	std::vector<AnimTransform> animTransforms;
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	KeyframeDesc keyframeDesc;

private:
	void CreateTexture();
	void CreateAnimationTransform(UINT index);

public: 
	ModelRenderer(std::shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	void SetModel(std::shared_ptr<Model> model);
	void SetPass(UINT8 pass) { this->pass = pass; }
	KeyframeDesc& GetKeyFrameDesc() { return keyframeDesc; }
	std::wstring GetModelName() const;

	InstanceID GetInstanceID() const;
	void RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer);
	void UpdateKeyframeDesc();

	virtual void Render() override;
};

