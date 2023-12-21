#include "pch.h"
#include "RenderManager.h"

#include "PipeLineHeaders.h"
#include "EngineHeader.h"
#include "ResourceHeader.h"
#include "EnumClass.h"

void RenderManager::Init(const std::shared_ptr<Shader>& shader)
{
	this->shader = shader;

	globalBuffer = std::make_shared<ConstantBuffer<GlobalDesc>>(Global::g_device, Global::g_immediateContext);
	globalBuffer->Create();
	globalEffectBuffer = shader->GetConstantBuffer("GlobalBuffer");	

	transformBuffer = std::make_shared<ConstantBuffer<TransformDesc>>(Global::g_device, Global::g_immediateContext);
	transformBuffer->Create();	
	transformEffectBuffer = shader->GetConstantBuffer("TransformBuffer");

	lightBuffer = std::make_shared<ConstantBuffer<LightDesc>>(Global::g_device, Global::g_immediateContext);
	lightBuffer->Create();
	lightEffectBuffer = shader->GetConstantBuffer("LightBuffer");

	materialBuffer = std::make_shared<ConstantBuffer<MaterialDesc>>(Global::g_device, Global::g_immediateContext);
	materialBuffer->Create();
	materialEffectBuffer = shader->GetConstantBuffer("MaterialBuffer");

	boneBuffer = std::make_shared<ConstantBuffer<BoneDesc>>(Global::g_device, Global::g_immediateContext);
	boneBuffer->Create();
	boneEffectBuffer = shader->GetConstantBuffer("BoneBuffer");

	keyframeBuffer = std::make_shared<ConstantBuffer<KeyframeDesc>>(Global::g_device, Global::g_immediateContext);
	keyframeBuffer->Create();
	keyframeEffectBuffer = shader->GetConstantBuffer("KeyframeBuffer");

	tweenBuffer = std::make_shared<ConstantBuffer<TweenDesc>>(Global::g_device, Global::g_immediateContext);
	tweenBuffer->Create();
	tweenEffectBuffer = shader->GetConstantBuffer("TweenBuffer");

	instancedTweenBuffer = std::make_shared<ConstantBuffer<InstancedTweenDesc>>(Global::g_device, Global::g_immediateContext);
	instancedTweenBuffer->Create();
	instancedTweenEffectBuffer = shader->GetConstantBuffer("InstancedTweenBuffer");
}

void RenderManager::Update()
{
	PushGlobalData(CameraManager::GetInstance().GetMainCamera()->viewMatrix
					, CameraManager::GetInstance().GetMainCamera()->projectionMatrix);
}

void RenderManager::SetShader(const std::shared_ptr<Shader>& shader)
{
	this->shader = shader;
}

void RenderManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	globalDesc.View = view;
	globalDesc.Projection = projection;
	globalDesc.VirwProjection = view * projection;
	globalDesc.ViewInverse = view.Invert();

	globalBuffer->CopyData(globalDesc);
	globalEffectBuffer->SetConstantBuffer(globalBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushTransformData(const TransformDesc& desc)
{
	transformDesc = desc;

	transformBuffer->CopyData(transformDesc);
	transformEffectBuffer->SetConstantBuffer(transformBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushLightData(const LightDesc& desc)
{
	lightDesc = desc;

	lightBuffer->CopyData(lightDesc);
	lightEffectBuffer->SetConstantBuffer(lightBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushMaterialData(const MaterialDesc& desc)
{
	materialDesc = desc;

	materialBuffer->CopyData(materialDesc);
	materialEffectBuffer->SetConstantBuffer(materialBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushBoneData(const BoneDesc& desc)
{
	boneDesc = desc;
	
	boneBuffer->CopyData(boneDesc);
	boneEffectBuffer->SetConstantBuffer(boneBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushKeyframeData(const KeyframeDesc& desc)
{
	keyframeDesc = desc;

	keyframeBuffer->CopyData(keyframeDesc);
	keyframeEffectBuffer->SetConstantBuffer(keyframeBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushTweenData(const TweenDesc& desc)
{
	tweenDesc = desc;

	tweenBuffer->CopyData(tweenDesc);
	tweenEffectBuffer->SetConstantBuffer(tweenBuffer->GetConstantBuffer().Get());
}

void RenderManager::PushInstancedTweenData(const InstancedTweenDesc& desc)
{
	instancedTweenDesc = desc;

	instancedTweenBuffer->CopyData(instancedTweenDesc);
	instancedTweenEffectBuffer->SetConstantBuffer(instancedTweenBuffer->GetConstantBuffer().Get());
}
