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
	std::shared_ptr<Texture> alphaTexture;
	std::shared_ptr<Texture> texture1;
	std::shared_ptr<Texture> texture2;
	std::shared_ptr<Texture> texture3;
	std::shared_ptr<Texture> texture4;

public:
	void TillingTexture(Vector3 centerPos, int tillingTexNum, std::vector<PNCTVertex>& vertexList, std::vector<UINT>& updateIdxList);
	void SetVertexByTexture(std::vector<PNCTVertex>& vertexList);
	void SaveAlphaTexture(std::wstring savePath);
	void SetSRV(std::shared_ptr<Shader> shader);

	void Init(SplattingDesc& desc);
};

