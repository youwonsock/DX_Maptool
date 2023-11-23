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

void GeometryHelper::CreateCube(std::shared_ptr<Geometry<VertexTextureData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<VertexTextureData> vtx(24);

	// æ’∏È
	vtx[0] = VertexTextureData{ Vector3(-w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vtx[1] = VertexTextureData{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 0.0f) };
	vtx[2] = VertexTextureData{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 0.0f) };
	vtx[3] = VertexTextureData{ Vector3(+w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	// µﬁ∏È
	vtx[4] = VertexTextureData{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 1.0f) };
	vtx[5] = VertexTextureData{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 1.0f) };
	vtx[6] = VertexTextureData{ Vector3(+w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vtx[7] = VertexTextureData{ Vector3(-w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	// ¿≠∏È
	vtx[8] = VertexTextureData{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 1.0f) };
	vtx[9] = VertexTextureData{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vtx[10] = VertexTextureData{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	vtx[11] = VertexTextureData{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 1.0f) };
	// æ∆∑ß∏È
	vtx[12] = VertexTextureData{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	vtx[13] = VertexTextureData{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vtx[14] = VertexTextureData{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 0.0f) };
	vtx[15] = VertexTextureData{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 0.0f) };
	// øﬁ¬ ∏È
	vtx[16] = VertexTextureData{ Vector3(-w2, -h2, +d2), Vector2(0.0f, 1.0f) };
	vtx[17] = VertexTextureData{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vtx[18] = VertexTextureData{ Vector3(-w2, +h2, -d2), Vector2(1.0f, 0.0f) };
	vtx[19] = VertexTextureData{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	// ø¿∏•¬ ∏È
	vtx[20] = VertexTextureData{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vtx[21] = VertexTextureData{ Vector3(+w2, +h2, -d2), Vector2(0.0f, 0.0f) };
	vtx[22] = VertexTextureData{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	vtx[23] = VertexTextureData{ Vector3(+w2, -h2, +d2), Vector2(1.0f, 1.0f) };

	geometry->SetVertices(vtx);

	std::vector<UINT> idx(36);

	// æ’∏È
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µﬁ∏È
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ¿≠∏È
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// æ∆∑ß∏È
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// øﬁ¬ ∏È
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ø¿∏•¬ ∏È
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(std::shared_ptr<Geometry<VertexTextureData>> geometry)
{
	float radius = 0.5f; // ±∏¿« π›¡ˆ∏ß
	UINT stackCount = 5; // ∞°∑Œ ∫–«“
	UINT sliceCount = 5; // ºº∑Œ ∫–«“

	std::vector<VertexTextureData> vtx;

	VertexTextureData v;

	// ∫œ±ÿ
	v.position = Vector3(0.0f, radius, 0.0f);
	v.UV = Vector2(0.5f, 0.0f);
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
	for (UINT y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
		for (UINT x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.UV = Vector2(deltaU * x, deltaV * y);

			vtx.push_back(v);
		}
	}

	// ≥≤±ÿ
	v.position = Vector3(0.0f, -radius, 0.0f);
	v.UV = Vector2(0.5f, 1.0f);
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	std::vector<UINT> idx(36);

	// ∫œ±ÿ ¿Œµ¶Ω∫
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ∏ˆ≈Î ¿Œµ¶Ω∫
	UINT ringVertexCount = sliceCount + 1;
	for (UINT y = 0; y < stackCount - 2; ++y)
	{
		for (UINT x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ≥≤±ÿ ¿Œµ¶Ω∫
	UINT bottomIndex = static_cast<UINT>(vtx.size()) - 1;
	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(std::shared_ptr<Geometry<VertexTextureData>> geometry, int sizeX, int sizeZ)
{
	std::vector<VertexTextureData> vtx;

	for (int z = 0; z < sizeZ + 1; z++)
	{
		for (int x = 0; x < sizeX + 1; x++)
		{
			VertexTextureData v;
			v.position = Vector3(static_cast<float>(x), 0, static_cast<float>(z));
			v.UV = Vector2(static_cast<float>(x), static_cast<float>(z));

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	std::vector<UINT> idx;

	for (int z = 0; z < sizeZ; z++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}
