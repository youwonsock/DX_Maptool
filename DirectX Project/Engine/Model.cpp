#include "pch.h"
#include "Model.h"

#include <filesystem>
#include "tinyxml2.h"
#include "ResourceHeader.h"
#include "PipeLineHeaders.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::BindCacheInfo()
{
	// Mesh에 Material 캐싱
	for (const auto& mesh : meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (root == nullptr && bones.size() > 0)
	{
		root = bones[0];

		for (const auto& bone : bones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = bones[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}
}

void Model::ReadMaterial(std::wstring filename)
{
	std::wstring fullPath = texturePath + filename + L".xml";
	auto parentPath = std::filesystem::path(fullPath).parent_path();

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(WStringToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(StringToWString(node->GetText()));

		// Diffuse Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			std::wstring textureStr = StringToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = ResourceManager::GetInstance().GetTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetDiffuseMap(texture);
			}
		}

		// Specular Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			std::wstring texture = StringToWString(node->GetText());
			if (texture.length() > 0)
			{
				std::wstring textureStr = StringToWString(node->GetText());
				if (textureStr.length() > 0)
				{
					auto texture = ResourceManager::GetInstance().GetTexture(textureStr, (parentPath / textureStr).wstring());
					material->SetSpecularMap(texture);
				}
			}
		}

		// Normal Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			std::wstring textureStr = StringToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = ResourceManager::GetInstance().GetTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetNormalMap(texture);
			}
		}

		// Ambient
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().ambient = color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().diffuse = color;
		}

		// Specular
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().specular = color;
		}

		// Emissive
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().emissive = color;
		}

		materials.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement();
	}

	BindCacheInfo();
}

void Model::ReadModel(std::wstring filename)
{
	std::wstring fullPath = modelPath + filename + L".mesh";

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	// Bones
	{
		const UINT count = file->Read<UINT>();
		
		for (UINT i = 0; i < count; i++)
		{
			std::shared_ptr<ModelBone> bone = std::make_shared<ModelBone>();
			bone->index = file->Read<int>();
			bone->name = StringToWString(file->Read<std::string>());
			bone->parentIndex = file->Read<int>();
			bone->transform = file->Read<Matrix>();
			bones.push_back(bone);
		}
	}

	// Mesh
	{
		const UINT count = file->Read<UINT>();

		for (UINT i = 0; i < count; i++)
		{
			std::shared_ptr<ModelMesh> mesh = std::make_shared<ModelMesh>();

			mesh->name = StringToWString(file->Read<std::string>());
			mesh->boneIndex = file->Read<int>();

			// Material
			mesh->materialName = StringToWString(file->Read<std::string>());

			//VertexData
			{
				const UINT count = file->Read<UINT>();
				std::vector<ModelVertexType> vertices;
				vertices.resize(count);

				void* data = vertices.data();
				file->Read(&data, sizeof(ModelVertexType) * count);
				mesh->geometry->AddVertices(vertices);
			}

			//IndexData
			{
				const UINT count = file->Read<UINT>();

				std::vector<UINT> indices;
				indices.resize(count);

				void* data = indices.data();
				file->Read(&data, sizeof(UINT) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			meshes.push_back(mesh);
		}
	}

	BindCacheInfo();
}

void Model::ReadAnimation(std::wstring filename)
{
	std::wstring fullPath = modelPath + filename + L".anim";

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	std::shared_ptr<ModelAnimation> animation = std::make_shared<ModelAnimation>();

	animation->name = StringToWString(file->Read<std::string>());
	animation->duration = file->Read<float>();
	animation->frameRate = file->Read<float>();
	animation->frameCount = file->Read<UINT>();

	UINT keyframeCount = file->Read<UINT>();

	for(UINT i = 0; i < keyframeCount; i++)
	{
		std::shared_ptr<ModelKeyframe> keyframe = std::make_shared<ModelKeyframe>();
		keyframe->boneName = StringToWString(file->Read<std::string>());
		UINT size = file->Read<UINT>();

		if (size > 0)
		{
			keyframe->transforms.resize(size);
			
			void* data = &keyframe->transforms[0];
			file->Read(&data, sizeof(ModelKeyframeData) * size);
		}

		animation->keyframes.insert(std::make_pair(keyframe->boneName, keyframe));
	}

	animations.push_back(animation);
}

std::shared_ptr<Material> Model::GetMaterialByName(const std::wstring& name)
{
	for (auto& material : materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

std::shared_ptr<ModelMesh> Model::GetMeshByName(const std::wstring& name)
{
	for (auto& mesh : meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

std::shared_ptr<ModelBone> Model::GetBoneByName(const std::wstring& name)
{
	for (auto& bone : bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

std::shared_ptr<ModelAnimation> Model::GetAnimationByName(const std::wstring& name)
{
	for(auto& animation : animations)
	{
		if (animation->name == name)
			return animation;
	}

	return nullptr;
}
