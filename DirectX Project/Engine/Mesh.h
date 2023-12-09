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

	std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

private:
	void CreateBuffers();

public:
	Mesh();
	~Mesh();

	virtual void CreateRactangle();
	virtual void CreateCube();
	virtual void CreateSphere();
	virtual void CreateGrid(UINT sizeX, UINT sizeY);
	
	virtual void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	virtual std::shared_ptr<VertexBuffer> GetVertexBuffer() const;
	
	virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
	virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
};

