#include "Sample.h"

#include "Engine/Global.h"

void Sample::Init()
{
	shader = std::make_shared<Shader>(L"01. Triangle.fx");

	vertices.resize(3);
	vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].position = Vector3(0.0f, 0.5f, 0.0f);
	vertices[2].position = Vector3(0.5f, -0.5f, 0.0f);


	buffer = std::make_shared<VertexBuffer>();
	buffer->CreateVertexBuffer(vertices);
}

void Sample::FixedUpdate()
{
}

void Sample::Update()
{
}

void Sample::PostUpdate()
{
}

void Sample::PreRender()
{
}

void Sample::Render()
{
	UINT stride = buffer->GetStride();
	UINT offset = buffer->GetOffset();

	Global::g_immediateContext->IASetVertexBuffers(0, 1, buffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	shader->Draw(1, 0, 3);
}

void Sample::PostRender()
{
}

void Sample::Release()
{
}