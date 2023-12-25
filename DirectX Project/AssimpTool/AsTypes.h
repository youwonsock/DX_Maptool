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

// animation
struct asBlendWeight
{
	Vector4 indices = Vector4(0,0,0,0); // 영향을 주는 본의 인덱스 4개
	Vector4 weights = Vector4(0,0,0,0); // 영향을 주는 본의 가중치 4개

	void Set(UINT index, UINT boneIndex, float weight)
	{
		switch (index)
		{
			case 0 : indices.x = boneIndex; weights.x = weight; break;
			case 1 : indices.y = boneIndex; weights.y = weight; break;
			case 2 : indices.z = boneIndex; weights.z = weight; break;
			case 3 : indices.w = boneIndex; weights.w = weight; break;
		}
	}
};

struct asBoneWeights
{
	using Pair = std::pair<int, float>;
	std::vector<Pair> boneWeights;

	void AddWeights(UINT boneIndex, float weight)
	{
		if(weight <= 0.0f)
			return;

		auto findInsertPos = std::find_if(boneWeights.begin(), boneWeights.end(), [weight](const Pair& pair)
												{ return weight > pair.second; });

		// 가중치가 높은 4개의 본만 사용하기 위해 높은 순으로 삽입
		boneWeights.insert(findInsertPos, Pair(boneIndex, weight));
	}

	// 영향을 주는 본의 개수가 다른 경우에도 최종 비율의 합을 1로 맞추기 위해 사용
	void Normalize()
	{
		if(boneWeights.size() >= 4)
			boneWeights.resize(4);

		float totalWeight = 0.0f;
		for (auto& boneWeight : boneWeights)
			totalWeight += boneWeight.second;

		float scale = 1.0f / totalWeight;
		for (auto& boneWeight : boneWeights)
			boneWeight.second /= scale;  // 비율을 1로 맞추기 위해 scale로 나눔
	}

	asBlendWeight GetBlendWeight()
	{
		asBlendWeight blendWeight;

		for (UINT i = 0; i < boneWeights.size(); i++)
		{
			if(i >= 4)
				break;

			blendWeight.Set(i, boneWeights[i].first, boneWeights[i].second);
		}

		return blendWeight;
	}
};

struct asKeyframeData
{
	float time = 0;
	Vector3 scale = {0,0,0};
	Quaternion rotation = {0,0,0,0};
	Vector3 translation = {0,0,0};
};

struct asKeyframe
{
	std::string boneName;
	std::vector<asKeyframeData> transforms;
};

struct asAnimation
{
	std::string name;
	UINT frameCount;
	float frameRate;
	float duration;

	std::vector<std::shared_ptr<asKeyframe>> keyframes;
};

// Cache
struct asAnimationNode
{
	aiString name;
	std::vector<asKeyframeData> keyframes;
};