#pragma once

enum HeightMode
{
	Mode_Flat,
	Mode_Round
};

class HeightMap
{
private:
	UINT rowNum;
	UINT colNum;
	float heightScale = 0.0f;

	HeightMode mode = HeightMode::Mode_Round;
	float changeHeight = 10.0f;

	std::shared_ptr<Texture> heightMapTexture = nullptr;

public:
	std::vector<float> heightList;

private:
	// change height map size to power of 2 + 1
	bool CheckSquare(int n);
	int ResizeMap(int n);

public:
	void ShowUI();
	void UpdateVertexHeight(std::vector<PNCTVertex>& vertexList, std::vector<UINT>& updateIdxList, Vector3 centerPos, float brushSize);
	void SaveHeightMap(std::wstring savePath);
	float GetHeightByRowCol(int row, int col);
	float GetHeightByIdx(UINT index);

	void Init(UINT& rowNum, UINT& colNum, float heightScale, std::wstring heightMapTexturePath);
};

