 #include "pch.h"
#include "Converter.h"

#include "Engine/tinyxml2.h"

Converter::Converter()
{
	importer = std::make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
}

void Converter::ReadAssetFile(const std::wstring& file)
{
	std::wstring fullPath = assetPath + file;

	auto p = std::filesystem::path(fullPath);
	
	if(!std::filesystem::exists(p))
		Utils::ShowErrorMessage(L"File not found: " + fullPath);

	scene = importer->ReadFile(
		Utils::WStringToString(fullPath),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if(scene == nullptr)
		Utils::ShowErrorMessage(L"Failed to load model: " + fullPath);
}

void Converter::ExportModelData(const std::wstring& savePath)
{
	std::wstring fullPath = modelPath + savePath + L".mesh";
	ReadModelData(scene->mRootNode, -1, -1);
	ReadSkinData();

	WriteModelFile(fullPath);
}

void Converter::ExportMaterialData(const std::wstring& savePath)
{
	std::wstring fullPath = texturePath + savePath + L".xml"; 
	ReadMaterialData();
	WirteMaterialData(fullPath);
}

void Converter::ExportAnimationData(const std::wstring& savePath, UINT index)
{
	std::wstring fullPath = modelPath + savePath + L".anim";
	assert(index < scene->mNumAnimations);

	std::shared_ptr<asAnimation> animation = ReadAnimationData(scene->mAnimations[index]);
	WriteAnimationData(animation, fullPath);
}

void Converter::ReadModelData(aiNode* node, int index, int parent)
{
	std::shared_ptr<asBone> bone = std::make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// Relative Transform
	Matrix transform(node->mTransformation[0]);
	bone->transform = transform.Transpose();

	// 2) Root (Local)
	Matrix matParent = Matrix::Identity;
	if (parent >= 0)
		matParent = bones[parent]->transform;

	// Local (Root) Transform
	bone->transform = bone->transform * matParent;

	bones.push_back(bone);

	// Mesh
	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadModelData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode* node, int bone)
{
	if (node->mNumMeshes < 1)
		return;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		std::shared_ptr<asMesh> mesh = std::make_shared<asMesh>();
		mesh->name = node->mName.C_Str();
		mesh->boneIndex = bone;

		UINT index = node->mMeshes[i];
		const aiMesh* srcMesh = scene->mMeshes[index];

		int temp = srcMesh->mMaterialIndex;

		// Material Name
		const aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		const UINT startVertex = mesh->vertices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			VertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vector3));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->vertices.push_back(vertex);
		}

		// Index
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->indices.push_back(face.mIndices[k] + startVertex);
		}

		meshes.push_back(mesh);
	}

	//meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	for (UINT i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* srcMesh = scene->mMeshes[i];

		if(srcMesh->HasBones() == false)
			continue;

		std::shared_ptr<asMesh> mesh = meshes[i];
		std::vector<asBoneWeights> tempVertexBoneWeights(mesh->vertices.size());

		// Bone을 순회하면서 연관된 VertexID, Weight를 기록
		for (UINT b = 0; b < srcMesh->mNumBones; ++b)
		{
			aiBone* srcMeshBone = srcMesh->mBones[b];
			UINT boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());

			for (UINT w = 0; w < srcMeshBone->mNumWeights; ++w)
			{
				UINT index = srcMeshBone->mWeights[w].mVertexId;
				float weight = srcMeshBone->mWeights[w].mWeight;  

				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}
		}

		for (UINT v = 0; v < tempVertexBoneWeights.size(); ++v)
		{
			tempVertexBoneWeights[v].Normalize();

			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeight();
			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
	}
}

void Converter::WriteModelFile(std::wstring filePath)
{
	auto path = std::filesystem::path(filePath);

	// 폴더가 없으면 만든다.
	std::filesystem::create_directory(path.parent_path());

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(filePath, FileMode::Write);

	// Bone Data
	file->Write<unsigned __int32>(bones.size());
	for (std::shared_ptr<asBone>& bone : bones)
	{
		file->Write<__int32>(bone->index);
		file->Write<std::string>(bone->name);
		file->Write<__int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	// Mesh Data
	file->Write<unsigned __int32>(meshes.size());
	for (std::shared_ptr<asMesh>& meshData : meshes)
	{
		file->Write<std::string>(meshData->name);
		file->Write<__int32>(meshData->boneIndex);
		file->Write<std::string>(meshData->materialName);

		// Vertex Data
		file->Write<unsigned __int32>(meshData->vertices.size());
		file->Write(&meshData->vertices[0], sizeof(VertexType) * meshData->vertices.size());

		// Index Data
		file->Write<unsigned __int32>(meshData->indices.size());
		file->Write(&meshData->indices[0], sizeof(unsigned __int32) * meshData->indices.size());
	}
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];

		std::shared_ptr<asMaterial> material = std::make_shared<asMaterial>();

		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		//ambient
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.f);

		//diffuse
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		//specular
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w); // specular power

		//emissive
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.f);

		aiString file;
		//diffuse texture
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		//specular texture
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		//normal texture
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		materials.push_back(material);
	}
}

void Converter::ReadKeyFrameData(std::shared_ptr<asAnimation> animation, aiNode* srcNode, std::unordered_map<std::string, std::shared_ptr<asAnimationNode>>& cache)
{
	std::shared_ptr<asKeyframe> keyframe = std::make_shared<asKeyframe>();
	keyframe->boneName = srcNode->mName.C_Str();

	std::shared_ptr<asAnimationNode> findNode = cache[srcNode->mName.C_Str()];

	for(UINT i = 0; i < animation->frameCount; ++i)
	{
		asKeyframeData frameData;

		if (findNode == nullptr)
		{
			Matrix transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (float)i;
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);
		}
		else
		{
			frameData = findNode->keyframes[i];
		}

		keyframe->transforms.push_back(frameData);
	}

	animation->keyframes.push_back(keyframe);

	for (UINT i = 0; i < srcNode->mNumChildren; ++i)
		ReadKeyFrameData(animation, srcNode->mChildren[i], cache);
}

void Converter::WriteAnimationData(std::shared_ptr<asAnimation> animation, std::wstring filePath)
{
	auto path = std::filesystem::path(filePath);

	std::filesystem::create_directory(path.parent_path());

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(filePath, FileMode::Write);

	file->Write<std::string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<UINT>(animation->frameCount);

	file->Write<UINT>(animation->keyframes.size());

	for (std::shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<std::string>(keyframe->boneName);

		file->Write<UINT>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

std::shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	std::shared_ptr<asAnimation> animation = std::make_shared<asAnimation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = (float)srcAnimation->mTicksPerSecond;
	animation->frameCount = (UINT)srcAnimation->mDuration + 1;

	std::unordered_map<std::string, std::shared_ptr<asAnimationNode>> cacheAnimNodes;
	for (UINT i = 0; i < srcAnimation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// 애니메이션 노드 데이터 파싱
		std::shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// 찾은 노드중 가장 긴 시간을 애니메이션의 길이로 설정
		animation->duration = max(animation->duration, node->keyframes.back().time);

		cacheAnimNodes.insert(std::make_pair(node->name.C_Str(), node));
	}

	ReadKeyFrameData(animation, scene->mRootNode, cacheAnimNodes);

	return animation;
}

std::shared_ptr<asAnimationNode> Converter::ParseAnimationNode(std::shared_ptr<asAnimation> animation, aiNodeAnim* srcNode)
{
	std::shared_ptr<asAnimationNode> node = std::make_shared<asAnimationNode>();
	node->name = srcNode->mNodeName;

	UINT keycount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys),srcNode->mNumRotationKeys);

	for (UINT k = 0; k < keycount; ++k)
	{
		asKeyframeData frameData;

		bool found = false;
		UINT t = node->keyframes.size();

		// Position
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}
		// Rotation
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = (float)key.mTime;

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}
		// Scale
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->keyframes.push_back(frameData);
	}

	// keyframe 늘려주기
	if (node->keyframes.size() < animation->frameCount)
	{
		UINT count = animation->frameCount - node->keyframes.size();

		asKeyframeData keyFrame;

		if(node->keyframes.size() > 0)
			keyFrame = node->keyframes.back();

		for (UINT i = 0; i < count; ++i)
			node->keyframes.push_back(keyFrame);
	}

	 return node;
}

UINT Converter::GetBoneIndex(std::string name)
{
	for (UINT i = 0; i < bones.size(); i++)
	{
		if (bones[i]->name == name)
			return bones[i]->index;
	}

	Utils::ShowErrorMessage(L"Bone not found: " + Utils::StringToWString(name));
	return -1;
}

void Converter::WirteMaterialData(std::wstring filePath)
{
	auto path = std::filesystem::path(filePath);

	// 폴더가 없으면 생성
	std::filesystem::create_directory(path.parent_path());
	std::string folder = path.parent_path().string();

	std::shared_ptr<tinyxml2::XMLDocument> document = std::make_shared<tinyxml2::XMLDocument>();
	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (std::shared_ptr<asMaterial> material : materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WirteTextureFile(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WirteTextureFile(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WirteTextureFile(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}

	document->SaveFile(Utils::WStringToString(filePath).c_str());
}

std::string Converter::WirteTextureFile(std::string saveFolder, std::string file)
{
	std::string fileName = std::filesystem::path(file).filename().string();
	std::string folderName = std::filesystem::path(saveFolder).filename().string();

	const aiTexture* srcTexture = scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		std::string pathStr  = (std::filesystem::path(saveFolder) / fileName).string();
		

		if (srcTexture->mHeight == 0)
		{
			std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
			file->Open(Utils::StringToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = Global::g_device->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			
			if(FAILED(hr))
				Utils::ShowErrorMessage(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(Global::g_device.Get(), Global::g_immediateContext.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::StringToWString(fileName).c_str());
			if (FAILED(hr))
				Utils::ShowErrorMessage(hr);
		}
	}
	else
	{
		std::string originStr = (std::filesystem::path(assetPath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		std::string pathStr = (std::filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;
}

