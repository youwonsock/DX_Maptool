#include "pch.h"
#include "CameraDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"

void CameraDemo::Init()
{
	shader = std::make_shared<Shader>(L"03. ConstBuffer.fx");

	geometry = std::make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateRectangle(geometry, Color(0, 1, 1, 1));

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

void CameraDemo::FixedUpdate()
{
}

void CameraDemo::Update()
{
	cameraObject->Update();
}

void CameraDemo::PostUpdate()
{
}

void CameraDemo::PreRender()
{
}

void CameraDemo::Render()
{
	shader->GetMatrix("World")->SetMatrix((float*)&world);
	shader->GetMatrix("View")->SetMatrix((float*)&Camera::viewMatrix);
	shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::projectionMatrix);

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,0, indexBuffer->GetIndexCount());
}

void CameraDemo::PostRender()
{
}

void CameraDemo::Release()
{
}
