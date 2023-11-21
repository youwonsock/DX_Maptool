#include "pch.h"
#include "Mesh.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Geometry.hpp"
#include "GeometryHelper.h"

Mesh::Mesh() : ResourceBase(ResourceType::Mesh)
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateDefaultRactangle()
{
	geometry = std::make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(geometry);

	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
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
