#include "pch.h"
#include "ModelMesh.h"

#include "Geometry.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

void ModelMesh::CreateBuffers()
{
	geometry = std::make_shared<Geometry<ModelVertexType>>();

	vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(geometry->GetVertices());

	indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(geometry->GetIndices());
}
