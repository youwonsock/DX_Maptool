#include "pch.h"
#include "ConstBufferDemo.h"

#include "Engine/Global.h"

void ConstBufferDemo::Init()
{
	shader = std::make_shared<Shader>(L"03. ConstBuffer.fx");

	geometry = std::make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateRectangle(geometry, Color(0, 1, 1, 1));

	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
}

void ConstBufferDemo::FixedUpdate()
{
}

void ConstBufferDemo::Update()
{
	double deltaTime = TimeManager::GetInstance().GetDeltaTime();

	if(InputManager::GetInstance().GetKeyState(DIK_A) == KeyState::HOLD)
	{
		translation.x -= 3 * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_D) == KeyState::HOLD)
	{
		translation.x += 3 * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_W) == KeyState::HOLD)
	{
		translation.y += 3 * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyState(DIK_S) == KeyState::HOLD)
	{
		translation.y -= 3 * deltaTime;
	}

	world = Matrix::CreateTranslation(translation);
}

void ConstBufferDemo::PostUpdate()
{
}

void ConstBufferDemo::PreRender()
{
}

void ConstBufferDemo::Render()
{
	shader->GetMatrix("World")->SetMatrix((float*)&world);
	shader->GetMatrix("View")->SetMatrix((float*)&view);
	shader->GetMatrix("Projection")->SetMatrix((float*)&projection);

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,1, indexBuffer->GetIndexCount());
}

void ConstBufferDemo::PostRender()
{
}

void ConstBufferDemo::Release()
{
}
