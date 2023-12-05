#include "pch.h"
#include "DivideTreeNode.h"

void DivideTreeNode::CreateVertexBuffer(std::vector<PNCTVertex>& verteces)
{
	// todo : check

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(PNCTVertex) * (UINT)verteces.size();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = verteces.data();

	Global::g_device->CreateBuffer(&desc, &subResource, vertexBuffer.GetAddressOf());
}

void DivideTreeNode::Update()
{
}

void DivideTreeNode::Render()
{
}
