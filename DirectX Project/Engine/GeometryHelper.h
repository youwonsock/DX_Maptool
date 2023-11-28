#pragma once

#include "VertexData.h"

template<typename T>
class Geometry;

class GeometryHelper
{
public:
	static void CreateRectangle(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateCube(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateSphere(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateGrid(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int sizeX, int sizeZ);
};

