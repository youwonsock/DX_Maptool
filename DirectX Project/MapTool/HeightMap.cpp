#include "pch.h"
#include "HeightMap.h"

bool HeightMap::CheckSquare(int n)
{
	return 	(n & (n - 1)) == 0;
}

int HeightMap::ResizeMap(int n)
{
	int result = 0;
	int N = 2;

	while (N < n)
	{
		result = N;
		N *= 2;
	}

	return result + 1;
}

float HeightMap::GetHeightByRowCol(int row, int col)
{
	return heightList[row * rowNum + col] * heightScale;
}

float HeightMap::GetHeightByIdx(UINT index)
{
	return heightList[index] * heightScale;
}

void HeightMap::Init(UINT& rowNum, UINT& colNum, float heightScale, std::wstring heightTexPath)
{
	heightMapTexture = std::make_unique<Texture>();
	if (heightMapTexture->Load(heightTexPath))
	{
		Vector2 size = heightMapTexture->GetSize();
		auto& info = heightMapTexture->GetInfo();
		auto mData = info->GetMetadata();
		auto images = info->GetImages();

		if (!CheckSquare(mData.width - 1))
			mData.width = ResizeMap(mData.width);
		if (!CheckSquare(mData.height - 1))
			mData.height = ResizeMap(mData.height);

		this->rowNum = rowNum = mData.height;
		this->colNum = colNum = mData.width;
		this->heightScale = heightScale;

		heightList.resize(rowNum * colNum);
		UCHAR* pTexels = (UCHAR*)images->pixels;

		for (UINT i = 0; i < rowNum; i++)
		{
			UINT rowStart = i * images->rowPitch;
			for (UINT j = 0; j < colNum; j++)
			{
				UINT colStart = j * 4;
				UINT uRed = pTexels[rowStart + colStart + 0];
				heightList[i * mData.width + j] = (float)uRed;
			}
		}
	}
	else
	{
		if (!CheckSquare(rowNum - 1))
			this->rowNum = rowNum = ResizeMap(rowNum);
		if (!CheckSquare(colNum - 1))
			this->colNum = colNum = ResizeMap(colNum);

		heightMapTexture->CreateTexture(rowNum, colNum);

		heightList.resize(rowNum * colNum);

		for (UINT i = 0; i < heightList.size(); i++)
			heightList[i] = 0.0f;
	}
}

void HeightMap::SaveHeightMap(std::wstring savePath)
{
	std::vector<BYTE> heightListByte;
	heightListByte.resize(rowNum * colNum * 4);

	for (int i = 0; i < rowNum * colNum; ++i)
	{
		heightListByte[i * 4 + 0] = (BYTE)heightList[i];
		heightListByte[i * 4 + 1] = (BYTE)heightList[i];
		heightListByte[i * 4 + 2] = (BYTE)heightList[i];
		heightListByte[i * 4 + 3] = (BYTE)heightList[i];
	}

	heightMapTexture->UpdateTexture(heightListByte);

	heightMapTexture->SaveTexture(savePath);
}
