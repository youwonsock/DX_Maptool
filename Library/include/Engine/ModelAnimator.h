#pragma once

#include "Component.h"

class Model;
class Shader;

struct AnimTransform
{
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

class ModelAnimator : public Component
{
private:
	using Base = Component;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Model> model;
	UINT pass = 0;

	std::vector<AnimTransform> animTransforms;
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	TweenDesc tweenDesc;

private:
	void CreateTexture();
	void CreateAnimationTransform(UINT index);

public:
	ModelAnimator(std::shared_ptr<Shader> shader);
	virtual ~ModelAnimator();

	void SetModel(std::shared_ptr<Model> model);
	void SetPass(UINT pass) { this->pass = pass; }
	TweenDesc& GetTweenDesc() { return tweenDesc; }

	InstanceID GetInstanceID() const;
	void RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer);
	void UpdateTweenData();
};

