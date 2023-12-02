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
		ShowErrorMessage(L"File not found: " + fullPath);

	scene = importer->ReadFile(
		WStringToString(fullPath),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if(scene == nullptr)
		ShowErrorMessage(L"Failed to load model: " + fullPath);
}

void Converter::ExportModelData(const std::wstring& savePath)
{
	std::wstring fullPath = modelPath + savePath + L".mesh";
	ReadModelData(scene->mRootNode, -1, -1);
	WriteModelFile(fullPath);
}

void Converter::ExportMaterialData(const std::wstring& savePath)
{
	std::wstring fullPath = texturePath + savePath + L".xml"; 
	ReadMaterialData();
	WirteMaterialData(fullPath);
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

	std::shared_ptr<asMesh> mesh = std::make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		const aiMesh* srcMesh = scene->mMeshes[index];

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
	}

	meshes.push_back(mesh);
}

void Converter::WriteModelFile(std::wstring filePath)
{
	auto path = std::filesystem::path(filePath);

	// 폴더가 없으면 만든다.
	std::filesystem::create_directory(path.parent_path());

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(filePath, FileMode::Write);

	OutputDebugString(std::to_wstring(bones.size()).c_str());

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
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		//specular
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w); // specular power

		//emissive
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
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

	document->SaveFile(WStringToString(filePath).c_str());
}

std::string Converter::WirteTextureFile(std::string saveFolder, std::string file)
{
	std::string fileName = std::filesystem::path(file).filename().string();
	std::string folderName = std::filesystem::path(saveFolder).filename().string();

	const aiTexture* srcTexture = scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		std::string pathStr = saveFolder + fileName;

		if (srcTexture->mHeight == 0)
		{
			std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
			file->Open(StringToWString(pathStr), FileMode::Write);
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
				ShowErrorMessage(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(Global::g_device.Get(), Global::g_immediateContext.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, StringToWString(fileName).c_str());
			if (FAILED(hr))
				ShowErrorMessage(hr);
		}
	}
	else
	{
		std::string originStr = (std::filesystem::path(assetPath) / folderName / file).string();
		Replace(originStr, "\\", "/");

		std::string pathStr = (std::filesystem::path(saveFolder) / fileName).string();
		Replace(pathStr, "\\", "/");

		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;
}
