#include "pch.h"
#include "TextureDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"
#include "Engine/ResourceBase.h"

void TextureDemo::Init()
{
	shader = std::make_shared<Shader>(L"04. World.fx");

	geometry = std::make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(geometry,10,10);
	texture = ResourceManager::GetInstance().Load<Texture>(L"Chim",L"../../Res/IMG/Chim.jpg");

	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());

	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform();
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());
}

void TextureDemo::FixedUpdate()
{
}

void TextureDemo::Update()
{
	cameraObject->Update();
}

void TextureDemo::PostUpdate()
{
}

void TextureDemo::PreRender()
{
}

void TextureDemo::Render()
{
	shader->GetMatrix("World")->SetMatrix((float*)&world);
	shader->GetMatrix("View")->SetMatrix((float*)&Camera::viewMatrix);
	shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::projectionMatrix);
	shader->GetSRV("Texture0")->SetResource(texture->GetShaderResourceView().Get());

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,0, indexBuffer->GetIndexCount());
}

void TextureDemo::PostRender()
{
}

void TextureDemo::Release()
{
}
