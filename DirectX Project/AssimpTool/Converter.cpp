#include "pch.h"
#include "Converter.h"

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
