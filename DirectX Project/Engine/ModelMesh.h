#pragma once

#include "VertexData.h"

template <typename T>
class Geometry;
class VertexBuffer;
class IndexBuffer;

struct ModelBone
{
	std::wstring name;
	int index;
	int parentIndex;
	std::shared_ptr<ModelBone> parent;

	Matrix transform;
	std::vector<std::shared_ptr<ModelBone>> children;
};

struct ModelMesh
{
public:
	std::wstring name;

	// Mesh
	std::shared_ptr<Geometry<ModelVertexType>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	// Material
	std::wstring materialName = L"";
	std::shared_ptr<Material> material; // Cache

	// Bones
	int boneIndex;
	std::shared_ptr<ModelBone> bone; // Cache;

private:
	void MakeGeometry();
public:
	ModelMesh() { MakeGeometry(); }
	~ModelMesh() {}
	
	void CreateBuffers();
};

