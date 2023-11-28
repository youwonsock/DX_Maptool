#include "pch.h"
#include "Mesh.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Geometry.hpp"
#include "GeometryHelper.h"

void Mesh::CreateBuffers()
{
	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
}

Mesh::Mesh() : ResourceBase(ResourceType::Mesh)
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateCube()
{
	geometry = std::make_shared<Geometry<VertexTextureNormalTangentData>>();
	GeometryHelper::CreateCube(geometry);
	CreateBuffers();
}

void Mesh::CreateSphere()
{
	geometry = std::make_shared<Geometry<VertexTextureNormalTangentData>>();
	GeometryHelper::CreateSphere(geometry);
	CreateBuffers();
}

void Mesh::CreateGrid(UINT sizeX, UINT sizeY)
{
	geometry = std::make_shared<Geometry<VertexTextureNormalTangentData>>();
	GeometryHelper::CreateGrid(geometry, sizeX, sizeY);
	CreateBuffers();
}

void Mesh::CreateRactangle()
{
	geometry = std::make_shared<Geometry<VertexTextureNormalTangentData>>();
	GeometryHelper::CreateRectangle(geometry);
	CreateBuffers();
}

void Mesh::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	this->vertexBuffer = vertexBuffer;
}

std::shared_ptr<VertexBuffer> Mesh::GetVertexBuffer() const
{
	return vertexBuffer;
}

void Mesh::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	this->indexBuffer = indexBuffer;
}

std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}
