#pragma once

#include "VertexData.h"

template<typename T>
class Geometry;

class GeometryHelper
{
public:
	static void CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> geometry, Color color);
	static void CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> geometry);
};

