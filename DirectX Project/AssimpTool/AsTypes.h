#pragma once

using VertexType = VertexTextureNormalTangentBlendData;

struct asBone
{
	std::string name;
	UINT index = -1;
	UINT parent = -1;
	
	Matrix transform;
};

struct asMesh
{
	std::string name;

	aiMesh* mesh;
	std::vector<VertexType> vertices;
	std::vector<UINT> indices;

	UINT boneIndex;
	std::string materialName;
};

struct asMaterial
{
	std::string name;
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;

	std::string diffuseFile;
	std::string specularFile;
	std::string normalFile;
};

