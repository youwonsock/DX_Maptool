#pragma once

#include "AsTypes.h"

class Converter_FBXSDK
{
private:
	FbxManager* pFbxManager = nullptr;
	FbxImporter* pFbxImporter = nullptr;
	FbxScene* pFbxScene = nullptr;
	FbxNode* pFbxRootNode = nullptr;

	std::map < std::string, FbxNode* > fbxNodeList;
	
	std::wstring assetPath = L"../../Res/Assets/";
	std::wstring modelPath = L"../../Res/Models/";
	std::wstring texturePath = L"../../Res/Textures/";

private:
	void WriteAnimationData(std::shared_ptr<asAnimation> animation, std::wstring filePath);

	void LoadFBX(std::wstring fileName);
	void PreProcess(FbxNode* fbxNode); 
	Matrix ConvertAMatrix(FbxAMatrix& fbxMatrix);
	Matrix ConvertDXMatrix(Matrix& dxMatrix);

	void GetAnimationData(std::shared_ptr<asAnimation>& animation);
public:
	void ReadAssetFile(const std::wstring& file);
	void ExportAnimationData(const std::wstring& savePath, UINT index = 0);

	Matrix GetGeometrMatrix(std::string nodeName);

	Converter_FBXSDK();
	~Converter_FBXSDK();
};

