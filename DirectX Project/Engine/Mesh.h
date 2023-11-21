#pragma once

#include "ResourceBase.h"
#include "VertexData.h"

class VertexBuffer;
class IndexBuffer;

template<typename T>
class Geometry;

class Mesh : public ResourceBase
{
private:
	using Base = ResourceBase;

	std::shared_ptr<Geometry<VertexTextureData>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

public:
	Mesh();
	~Mesh();

	void CreateDefaultRactangle();

	void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	std::shared_ptr<VertexBuffer> GetVertexBuffer() const;

	void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
};

