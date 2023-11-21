#pragma once

#include "Singleton.hpp"
#include "Struct.h"

class Pipeline;
class RasterizerState;
class SamplerState;
class BlendState;
class GameObject;

template <typename T>
class ConstantBuffer;

class RenderManager : public Singleton<RenderManager>
{
private:
	friend class Singleton<RenderManager>;
	RenderManager() {};

	std::shared_ptr<Pipeline> pipeline;
	std::shared_ptr<RasterizerState> rasterizerState;
	std::shared_ptr<SamplerState> samplerState;
	std::shared_ptr<BlendState> blendState;

	CameraData cameraData;
	std::shared_ptr<ConstantBuffer<CameraData>> cameraBuffer;

	TransformData transformData;
	std::shared_ptr<ConstantBuffer<TransformData>> transformBuffer;

	AnimationData animationData;
	std::shared_ptr<ConstantBuffer<AnimationData>> animationBuffer;

	std::vector<std::shared_ptr<GameObject>> renderObjects;

private:
	void PushCameraData();
	void PushTransformData();
	void PushAnimationData();

	void GetherRenderObjects();
	void RenderObjects();

public:
	void Init();
	void Render();
};

