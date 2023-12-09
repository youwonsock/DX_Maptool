#include "pch.h"
#include "RenderMgr.h"

void RenderMgr::Init(std::shared_ptr<Shader> shader)
{
	this->shader = shader;

	globalBuffer = std::make_shared<ConstantBuffer<GlobalDesc>>(Global::g_device, Global::g_immediateContext);
	globalBuffer->Create();
	globalEffectBuffer = shader->GetConstantBuffer("GlobalBuffer");

	transformBuffer = std::make_shared<ConstantBuffer<TransformDesc>>(Global::g_device, Global::g_immediateContext);
	transformBuffer->Create();
	transformEffectBuffer = shader->GetConstantBuffer("TransformBuffer");
}

void RenderMgr::Update()
{
	globalDesc.View = Camera::viewMatrix;
	globalDesc.Projection = Camera::projectionMatrix;
	globalDesc.VirwProjection = Camera::viewMatrix * Camera::projectionMatrix;
	globalDesc.ViewInverse = globalDesc.View.Invert();

	globalBuffer->CopyData(globalDesc);
	globalEffectBuffer->SetConstantBuffer(globalBuffer->GetConstantBuffer().Get());


	transformDesc.World = Matrix::Identity;
	
	transformBuffer->CopyData(transformDesc);
	transformEffectBuffer->SetConstantBuffer(transformBuffer->GetConstantBuffer().Get());
}

RenderMgr::RenderMgr()
{
}

RenderMgr::~RenderMgr()
{
}
