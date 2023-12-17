#include "pch.h"
#include "SectionNode.h"

void SectionNode::Render()
{
	if (vertexBuffer == nullptr || indexBuffer == nullptr || shader == nullptr)
		return;

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, indexBuffer->GetIndexCount());
}

void SectionNode::SetVertexBuffer()
{
	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vertices, 0, true,false);
}

void SectionNode::UpdateVertexBuffer()
{
	vertexBuffer->UpdateVertexBuffer(vertices);
}
