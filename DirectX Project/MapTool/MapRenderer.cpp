#include "pch.h"
#include "MapRenderer.h"

void MapRenderer::Update()
{
	globalDesc.View = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	globalDesc.Projection = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;
	globalDesc.VirwProjection = globalDesc.View * globalDesc.Projection;
	globalDesc.ViewInverse = globalDesc.View.Invert();

	globalBuffer->CopyData(globalDesc);
	globalEffectBuffer->SetConstantBuffer(globalBuffer->GetConstantBuffer().Get());
}

void MapRenderer::Init()
{
	shader = ResourceManager::GetInstance().Get<Shader>(L"MapToolShader");

	globalBuffer = std::make_shared<ConstantBuffer<GlobalDesc>>(Global::g_device, Global::g_immediateContext);
	globalBuffer->Create();
	globalEffectBuffer = shader->GetConstantBuffer("GlobalBuffer");

	transformBuffer = std::make_shared<ConstantBuffer<TransformDesc>>(Global::g_device, Global::g_immediateContext);
	transformBuffer->Create();
	transformEffectBuffer = shader->GetConstantBuffer("TransformBuffer");

	transformDesc.World = Matrix::Identity;

	transformBuffer->CopyData(transformDesc);
	transformEffectBuffer->SetConstantBuffer(transformBuffer->GetConstantBuffer().Get());
}

MapRenderer::MapRenderer()
{
}

MapRenderer::~MapRenderer()
{
}
