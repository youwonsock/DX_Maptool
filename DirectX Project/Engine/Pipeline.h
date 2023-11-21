#pragma once

#include "EnumClass.h"
#include "ConstantBuffer.h"

class InputLayout;
class VertexShader;
class PixelShader;
class RasterizerState;
class BlendState;

class VertexBuffer;
class IndexBuffer;
class Texture;
class SamplerState;

struct PipelineInfo
{
	std::shared_ptr<InputLayout> inputLayout;
	std::shared_ptr<VertexShader> vertexShader;
	std::shared_ptr<PixelShader> pixelShader;
	std::shared_ptr<RasterizerState> rasterizerState;
	std::shared_ptr<BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
private:
	ComPtr<ID3D11DeviceContext> immediateContext;
public:
	Pipeline(ComPtr<ID3D11DeviceContext> immediateContext)
	{
		this->immediateContext = immediateContext;
	}
	~Pipeline() {};

public:
	void UpdatePipeline(const PipelineInfo& pipelineInfo);

	void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	template <typename T>
	void SetConstantBuffer(const std::shared_ptr<ConstantBuffer<T>>& constantBuffer, UINT slot, UINT scope)
	{
		if(scope & ShaderScope::SS_VertexShader)
			immediateContext->VSSetConstantBuffers(slot, 1, constantBuffer->GetConstantBuffer().GetAddressOf());

		if (scope & ShaderScope::SS_PixelShader)
			immediateContext->PSSetConstantBuffers(slot, 1, constantBuffer->GetConstantBuffer().GetAddressOf());
	}

	void SetTexture(UINT slot, UINT scope, const std::shared_ptr<Texture>& texture);
	void SetSamplerState(UINT slot, UINT scope, const std::shared_ptr<SamplerState>& samplerState);

	void Draw(UINT vertexCount, UINT startVertexLocation);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);
};

