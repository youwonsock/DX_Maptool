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
}

void RenderManager::Update()
{
	PushGlobalData(Camera::viewMatrix, Camera::projectionMatrix);
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
