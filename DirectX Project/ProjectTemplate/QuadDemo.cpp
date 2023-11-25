#include "pch.h"
#include "QuadDemo.h"

#include "Engine/Global.h"

void QuadDemo::Init()
{
	shader = std::make_shared<Shader>(L"02. Quad.fx");

	geometry = std::make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateRectangle(geometry, Color(0, 1, 1, 1));

	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
}

void QuadDemo::FixedUpdate()
{
}

void QuadDemo::Update()
{
}

void QuadDemo::PostUpdate()
{
}

void QuadDemo::PreRender()
{
}

void QuadDemo::Render()
{
	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,1, indexBuffer->GetIndexCount());
}

void QuadDemo::PostRender()
{
}

void QuadDemo::Release()
{
}
