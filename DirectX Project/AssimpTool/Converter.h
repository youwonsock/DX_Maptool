#pragma once
class Converter
{
private:
	std::shared_ptr<Assimp::Importer> importer;
	const aiScene* scene;

	std::wstring assetPath = L"../../Res/Assets/";
	std::wstring modelPath = L"../../Res/Models/";
	std::wstring texturePath = L"../../Res/Texture/";
public:
	Converter();
	~Converter();

	void ReadAssetFile(const std::wstring& file);
};

