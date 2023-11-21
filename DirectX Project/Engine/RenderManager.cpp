#include "pch.h"
#include "RenderManager.h"

#include "PipeLineHeaders.h"
#include "EngineHeader.h"
#include "ResourceHeader.h"
#include "EnumClass.h"
#include "Global.h"

void RenderManager::PushCameraData()
{
	cameraData.viewMatrix = Camera::viewMatrix;
	cameraData.projectionMatrix = Camera::projectionMatrix;
	cameraBuffer->CopyData(cameraData);
}

void RenderManager::PushTransformData()
{
	transformBuffer->CopyData(transformData);
}

void RenderManager::PushAnimationData()
{
	animationBuffer->CopyData(animationData);
}

void RenderManager::GetherRenderObjects()
{
	renderObjects.clear();

	auto& gameObjects = SceneManager::GetInstance().GetCurrentScene()->GetGameObjectArr();

	for (auto& gameObject : gameObjects)
	{
		auto meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer != nullptr)
			renderObjects.push_back(gameObject);
	}
}

void RenderManager::RenderObjects()
{
	for (auto& gameObject : renderObjects)
	{
		auto meshRenderer = gameObject->GetMeshRenderer();
		auto transform = gameObject->GetTransform();

		// srt
		transformData.worldMatrix = transform->GetWorldMatrix();
		PushTransformData();

		// animation
		auto animator = gameObject->GetAnimator();
		if (animator != nullptr)
		{
			const KeyFrame keyFrame = animator->GetCurrentKeyFrame();

			animationData.spriteOffset = keyFrame.offset;
			animationData.spriteSize = keyFrame.size;
			animationData.textureSize = animator->GetAnimation()->GetTextureSize();
			animationData.useAnimation = 1.0f;
			PushAnimationData();

			pipeline->SetConstantBuffer(animationBuffer, 2, SS_VertexShader);
			pipeline->SetTexture(0, SS_PixelShader, animator->GetAnimation()->GetTexture());
		}
		else
		{
			animationData.spriteOffset = Vector2(0,0);
			animationData.spriteSize = Vector2(0, 0);
			animationData.textureSize = Vector2(0, 0);
			animationData.useAnimation = 0;
			PushAnimationData();

			pipeline->SetConstantBuffer(animationBuffer, 2, SS_VertexShader);
			pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		}

		PipelineInfo info;
		info.inputLayout = meshRenderer->GetInputLayout();
		info.vertexShader = meshRenderer->GetVertexShader();
		info.pixelShader = meshRenderer->GetPixelShader();
		info.rasterizerState = rasterizerState;
		info.blendState = blendState;
		pipeline->UpdatePipeline(info);

		pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());

		pipeline->SetConstantBuffer(cameraBuffer, 0, SS_VertexShader);
		pipeline->SetConstantBuffer(transformBuffer, 1, SS_VertexShader);

		//_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		pipeline->SetSamplerState(0, SS_PixelShader, samplerState);

		pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetIndexCount(), 0, 0);
	}
}

void RenderManager::Init()
{

	pipeline = std::make_shared<Pipeline>(Global::g_immediateContext);

	cameraBuffer = std::make_shared<ConstantBuffer<CameraData>>(Global::g_device, Global::g_immediateContext);
	cameraBuffer->Create();
	transformBuffer = std::make_shared<ConstantBuffer<TransformData>>(Global::g_device, Global::g_immediateContext);
	transformBuffer->Create();
	animationBuffer = std::make_shared<ConstantBuffer<AnimationData>>(Global::g_device, Global::g_immediateContext);
	animationBuffer->Create();

	rasterizerState = std::make_shared<RasterizerState>();
	rasterizerState->CreateRasterizerState();
	blendState = std::make_shared<BlendState>();
	blendState->CreateBlendState();
	samplerState = std::make_shared<SamplerState>();
	samplerState->CreateSamplerState();
}

void RenderManager::Render()
{
	PushCameraData();
	GetherRenderObjects();
	RenderObjects();
}
