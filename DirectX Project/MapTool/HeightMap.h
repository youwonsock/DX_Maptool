#pragma once

class HeightMap
{
private:
	std::shared_ptr<Texture> heightMapTexture;

	UINT rowNum;
	UINT colNum;
	float heightScale = 0.0f;

public:
	std::vector<float> heightList;

private:
	// change height map size to power of 2 + 1
	bool CheckSquare(int n);
	int ResizeMap(int n);

public:

	void Init(UINT& rowNum, UINT& colNum, float heightScale, std::wstring heightTexPath);
	void SaveHeightMap(std::wstring savePath);
	float GetHeightByRowCol(int row, int col);
	float GetHeightByIdx(UINT index);
};

