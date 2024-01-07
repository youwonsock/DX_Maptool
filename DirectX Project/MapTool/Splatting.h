#pragma once

struct SplattingDesc
{
	std::wstring alphaTexPath = L"";

	std::wstring texture1Path = L"";
	std::wstring texture2Path = L"";
	std::wstring texture3Path = L"";
	std::wstring texture4Path = L"";

	int rowNum = 0;
	int colNum = 0;
};

class Splatting
{
private:
	std::shared_ptr<Shader> shader;

	std::shared_ptr<Texture> alphaTexture = nullptr;
	std::shared_ptr<Texture> texture1 = nullptr;
	std::shared_ptr<Texture> texture2 = nullptr;
	std::shared_ptr<Texture> texture3 = nullptr;
	std::shared_ptr<Texture> texture4 = nullptr;

	int rowNum = 0;
	int colNum = 0;

	float brushScale = 500.0f;
	int tillingTexNum = 0;

private:
	void SetSRV();

public:
	void TillingTexture(Vector3 centerPos, float brushSize, std::vector<PNCTVertex>& vertexList, std::vector<UINT>& updateIdxList);
	void SetVertexByTexture(std::vector<PNCTVertex>& vertexList);
	void SaveAlphaTexture(std::wstring savePath);
	void ShowUI();

	void Init(std::wstring filePath, int rowNum, int colNum);

	void Save(std::wstring filePath, std::wstring alphaTexturePath);
	void Load(std::wstring filePath);
};

