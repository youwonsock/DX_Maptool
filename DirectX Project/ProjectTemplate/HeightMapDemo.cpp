#include "HeightMapDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"
#include "Engine/ResourceBase.h"

void HeightMapDemo::Init()
{
	shader = std::make_shared<Shader>(L"05. Sampler.fx");

	texture = ResourceManager::GetInstance().Load<Texture>(L"Chim",L"../../Res/IMG/Chim.jpg");
	heightMap = ResourceManager::GetInstance().Load<Texture>(L"HeightMap", L"../../Res/IMG/Terrain/height.png");
	
	Vector2 size = heightMap->GetSize();

	geometry = std::make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(geometry,256,256);



	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());

	// camera
	cameraObject = std::make_shared<GameObject>();
	cameraObject->GetTransform();
	cameraObject->AddComponent(std::make_shared<Camera>());
	cameraObject->AddComponent(std::make_shared<TestCamera>());
	cameraObject->GetTransform()->SetWorldPosition(Vector3(5, 10, -10));
	cameraObject->GetTransform()->SetWorldRotation(Vector3(-10, 0, 0));

	
}

void HeightMapDemo::FixedUpdate()
{
}

void HeightMapDemo::Update()
{
	cameraObject->Update();
}

void HeightMapDemo::PostUpdate()
{
}

void HeightMapDemo::PreRender()
{
}

void HeightMapDemo::Render()
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
	shader->GetScalar("Address")->SetInt(AddressMode::Border);

	UINT stride = vertexBuffer->GetStride();
	UINT offset = vertexBuffer->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0,0, indexBuffer->GetIndexCount());
}

void HeightMapDemo::PostRender()
{
}

void HeightMapDemo::Release()
{
}
