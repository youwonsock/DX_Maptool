#include "pch.h"
#include "ModelMesh.h"

#include "Geometry.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

void ModelMesh::CreateBuffers()
{
	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
}

void ModelMesh::MakeGeometry()
{
	geometry = std::make_shared<Geometry<ModelVertexType>>();
}
