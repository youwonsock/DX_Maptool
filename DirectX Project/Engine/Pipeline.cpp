#include "pch.h"
#include "Pipeline.h"

#include "PipeLineHeaders.h"
#include "Texture.h"

void Pipeline::UpdatePipeline(const PipelineInfo& pipelineInfo)
{
	// Input assembler
	Global::g_immediateContext->IASetInputLayout(pipelineInfo.inputLayout->GetInputLayout().Get());
	Global::g_immediateContext->IASetPrimitiveTopology(pipelineInfo.primitiveTopology);

	// Vertex shader
	if(pipelineInfo.vertexShader != nullptr)
		Global::g_immediateContext->VSSetShader(pipelineInfo.vertexShader->GetShader().Get(), nullptr, 0);
	
	// Rasterizer State
	if(pipelineInfo.rasterizerState != nullptr)
		Global::g_immediateContext->RSSetState(pipelineInfo.rasterizerState->GetRasterizerState().Get());

	// Pixel shader
	if(pipelineInfo.pixelShader != nullptr)
		Global::g_immediateContext->PSSetShader(pipelineInfo.pixelShader->GetShader().Get(), nullptr, 0);
	
	// Output merger
	if (pipelineInfo.blendState != nullptr)
		Global::g_immediateContext->OMSetBlendState(pipelineInfo.blendState->GetBlendState().Get()
												, &pipelineInfo.blendState->GetBlendFactor(), 0xffffffff);

}

void Pipeline::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(UINT slot, UINT scope, const std::shared_ptr<Texture>& texture)
{
	if(scope & ShaderScope::SS_VertexShader)
		Global::g_immediateContext->VSSetShaderResources(slot, 1, texture->GetShaderResourceView().GetAddressOf());

	if (scope & ShaderScope::SS_PixelShader)
		Global::g_immediateContext->PSSetShaderResources(slot, 1, texture->GetShaderResourceView().GetAddressOf());
}

void Pipeline::SetSamplerState(UINT slot, UINT scope, const std::shared_ptr<SamplerState>& samplerState)
{
	if (scope & ShaderScope::SS_VertexShader)
		Global::g_immediateContext->VSSetSamplers(slot, 1, samplerState->GetSamplerState().GetAddressOf());

	if (scope & ShaderScope::SS_PixelShader)
		Global::g_immediateContext->PSSetSamplers(slot, 1, samplerState->GetSamplerState().GetAddressOf());
}

void Pipeline::Draw(UINT vertexCount, UINT startVertexLocation)
{
	Global::g_immediateContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	Global::g_immediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

