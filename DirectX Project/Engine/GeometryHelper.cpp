#include "pch.h"
#include "GeometryHelper.h"
#include "Geometry.hpp"

void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	std::vector<VertexColorData> vertices =
	{
		VertexColorData(Vector3(-0.5f, -0.5f, 0.0f), color),
		VertexColorData(Vector3(-0.5f, 0.5f, 0.0f), color),
		VertexColorData(Vector3(0.5f, 0.5f, 0.0f), color),
		VertexColorData(Vector3(0.5f, -0.5f, 0.0f), color)
	};
	geometry->SetVertices(vertices);

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 2, 3
	};
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> geometry)
{
std::vector<VertexTextureData> vertices =
	{
		VertexTextureData(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)),
		VertexTextureData(Vector3(-0.5f, 0.5f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexTextureData(Vector3(0.5f, 0.5f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexTextureData(Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f))
	};
	geometry->SetVertices(vertices);

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 2, 3
	};
	geometry->SetIndices(indices);
}
