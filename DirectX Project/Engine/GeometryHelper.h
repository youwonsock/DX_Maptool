#pragma once

#include "VertexData.h"

template<typename T>
class Geometry;

class GeometryHelper
{
public:
	static void CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> geometry, Color color);
	
	static void CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateCube(std::shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateSphere(std::shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateGrid(std::shared_ptr<Geometry<VertexTextureData>> geometry, int sizeX, int sizeZ);
};

