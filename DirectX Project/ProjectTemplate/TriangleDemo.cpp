#include "pch.h"
#include "TriangleDemo.h"

#include "Engine/Global.h"

void TriangleDemo::Init()
{
	shader = std::make_shared<Shader>(L"01. Triangle.fx");

	vertices.resize(3);
	vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].position = Vector3(0.0f, 0.5f, 0.0f);
	vertices[2].position = Vector3(0.5f, -0.5f, 0.0f);


	buffer = std::make_shared<VertexBuffer>();
	buffer->CreateVertexBuffer(vertices);
}

void TriangleDemo::FixedUpdate()
{
}

void TriangleDemo::Update()
{
}

void TriangleDemo::PostUpdate()
{
}

void TriangleDemo::PreRender()
{
}

void TriangleDemo::Render()
{
	UINT stride = buffer->GetStride();
	UINT offset = buffer->GetOffset();

	Global::g_immediateContext->IASetVertexBuffers(0, 1, buffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	shader->Draw(1, 0, 3);
}

void TriangleDemo::PostRender()
{
}

void TriangleDemo::Release()
{
}