 #pragma once

#include "AsTypes.h"

class Converter
{
private:
	std::shared_ptr<Assimp::Importer> importer;
	const aiScene* scene;

	std::wstring assetPath = L"../../Res/Assets/";
	std::wstring modelPath = L"../../Res/Models/";
	std::wstring texturePath = L"../../Res/Textures/";

	std::vector<std::shared_ptr<asMaterial>> materials;
	std::vector<std::shared_ptr<asMesh>> meshes;
	std::vector<std::shared_ptr<asBone>> bones;
private:
	void ReadModelData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void ReadSkinData();
	void ReadMaterialData(); 
	void ReadKeyFrameData(std::shared_ptr<asAnimation> animation,aiNode* srcNode, std::unordered_map<std::string, std::shared_ptr<asAnimationNode>>& cache);
	std::shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);

	std::shared_ptr<asAnimationNode> ParseAnimationNode(std::shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);
	UINT GetBoneIndex(std::string name);  

	void WriteModelFile(std::wstring filePath);
	void WirteMaterialData(std::wstring filePath);
	std::string WirteTextureFile(std::string saveFolder, std::string file);
	void WriteAnimationData(std::shared_ptr<asAnimation> animation, std::wstring filePath);

public:
	Converter();
	~Converter();

	void ReadAssetFile(const std::wstring& file);
	void ExportModelData(const std::wstring& savePath);
	void ExportMaterialData(const std::wstring& savePath);
	void ExportAnimationData(const std::wstring& savePath, UINT index = 0);
};

