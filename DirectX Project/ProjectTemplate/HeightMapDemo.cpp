#include "pch.h"
#include "HeightMapDemo.h"

#include "Engine/Global.h"
#include "TestCamera.h"
#include "Engine/ResourceBase.h"

void HeightMapDemo::Init()
{
	shader = std::make_shared<Shader>(L"06. Terrain.fx");

	texture = ResourceManager::GetInstance().Load<Texture>(L"Chim",L"../../Res/IMG/Chim.jpg");
	heightMap = ResourceManager::GetInstance().Load<Texture>(L"HeightMap", L"../../Res/IMG/Terrain/height.png");
	
	Vector2 size = heightMap->GetSize();
	auto info = heightMap->GetInfo();
	uint8_t* pixelBuffer = info->GetPixels();

	geometry = std::make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(geometry,size.x, size.y);

	// cpu
	{
		std::vector<VertexTextureData>& vertices = const_cast<std::vector<VertexTextureData>&>(geometry->GetVertices());
	
		for (int z = 0; z < size.y; z++)
		{
			for (int x = 0; x < size.x; x++)
			{
				int index = z * size.x + x;

				UINT height = pixelBuffer[index] / 255.f * 25.f;
				vertices[index].position.y = height;
			}
		}
	}

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
