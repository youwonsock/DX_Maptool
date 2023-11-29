#pragma once

#include "AsTypes.h"

class Converter
{
private:
	std::shared_ptr<Assimp::Importer> importer;
	const aiScene* scene;

	std::wstring assetPath = L"../../Res/Assets/";
	std::wstring modelPath = L"../../Res/Models/";
	std::wstring texturePath = L"../../Res/Texture/";

	std::vector<std::shared_ptr<asMaterial>> materials;
	std::vector<std::shared_ptr<asMesh>> meshes;
	std::vector<std::shared_ptr<asBone>> bones;
private:
	void ReadModelData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void ReadMaterialData();

	void WriteModelFile(std::wstring filePath);
	void WirteMaterialData(std::wstring filePath);
	std::string WirteTextureFile(std::string saveFolder, std::string file);

public:
	Converter();
	~Converter();

	void ReadAssetFile(const std::wstring& file);
	void ExportModelData(const std::wstring& savePath);
	void ExportMaterialData(const std::wstring& savePath);
};

