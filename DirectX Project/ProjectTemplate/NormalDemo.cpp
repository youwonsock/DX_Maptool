#include "pch.h"
#include "NormalDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"
#include "Engine/ResourceBase.h"

void NormalDemo::Init()
{
	shader = std::make_shared<Shader>(L"07. Normal.fx");

	texture = ResourceManager::GetInstance().Load<Texture>(L"Chim",L"../../Res/IMG/Chim.jpg");

	geometry = std::make_shared<Geometry<VertexTextureNormalData>>();
	//GeometryHelper::CreateCube(geometry);
	GeometryHelper::CreateSphere(geometry);

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

void NormalDemo::FixedUpdate()
{
}

void NormalDemo::Update()
{
	cameraObject->Update();
}

void NormalDemo::PostUpdate()
{
}

void NormalDemo::PreRender()
{
}

void NormalDemo::Render()
{
	enum AddressMode
	{
		Wrap = 0,
		Mirror,
		Clamp,
		Border,
	};

	shader->GetMatrix("World")->SetMatrix((float*)&world);
	shader->GetMatrix("View")->SetMatrix((float*)&Camera::viewMatrix);
	shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::projectionMatrix);
	shader->GetSRV("Texture0")->SetResource(texture->GetShaderResourceView().Get());
	shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,0, indexBuffer->GetIndexCount());
}

void NormalDemo::PostRender()
{
}

void NormalDemo::Release()
{
}
