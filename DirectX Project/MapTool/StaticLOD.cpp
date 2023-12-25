#include "pch.h"
#include "StaticLOD.h"

#include "SectionNode.h"

void StaticLOD::SetLod(DWORD width, DWORD numDepth)
{
	cellNum = (int)((width - 1) / pow(2.0f, (float)numDepth));
	patchLodCount = (int)((log((float)cellNum) / log(2.0f)));

	ComputeStaticLodIndex();
}

std::shared_ptr<IndexBuffer>& StaticLOD::GetLodIndexBuffer(std::shared_ptr<SectionNode>& pNode)
{
	GetLodSubIndex(pNode);
	GetLodType(pNode);

	return lodDataList[pNode->lodLevel]->indexBufferList[pNode->lodType];
}

void StaticLOD::GetLodSubIndex(std::shared_ptr<SectionNode> pNode)
{
	int iNumPatchIndex = cellNum;

	float fRatio = GetExpansionRatio(pNode->boundingBox.center);
	DWORD dwCurentRatio = (DWORD)(fRatio * patchLodCount);
	DWORD dwMaxRatio = (DWORD)(fRatio * patchLodCount + 0.5f);
	DWORD dwMinRatio = (DWORD)(fRatio * patchLodCount - 0.5f);

	pNode->lodLevel = dwCurentRatio;
	
	if ((DWORD)patchLodCount < pNode->lodLevel)
		pNode->lodLevel -= 1;
}

void StaticLOD::GetLodType(std::shared_ptr<SectionNode> pNode)
{
	if (pNode->neighborNodeList.size() <= 0)
	{
		pNode->lodType = 0;
		return;
	}

	DWORD dwType = 0;
	if (pNode->neighborNodeList[0] && pNode->neighborNodeList[0]->lodLevel < pNode->lodLevel) dwType += 1;
	if (pNode->neighborNodeList[1] && pNode->neighborNodeList[1]->lodLevel < pNode->lodLevel) dwType += 4;
	if (pNode->neighborNodeList[2] && pNode->neighborNodeList[2]->lodLevel < pNode->lodLevel) dwType += 8;
	if (pNode->neighborNodeList[3] && pNode->neighborNodeList[3]->lodLevel < pNode->lodLevel) dwType += 2;

	pNode->lodType = dwType;
}

DWORD StaticLOD::GetIndexCount(DWORD data, DWORD cellNum)
{
	DWORD	numFaces = (cellNum * cellNum * 4);

	DWORD	levelCount = 0;
	if (data & 8) levelCount++;
	if (data & 4) levelCount++;
	if (data & 2) levelCount++;
	if (data & 1) levelCount++;

	numFaces = (numFaces + (cellNum * levelCount));
	return numFaces * 3;
}

void StaticLOD::ComputeStaticLodIndex()
{
	lodDataList.resize(patchLodCount + 1);

	for (int i = 0; i < lodDataList.size(); ++i)
	{
		lodDataList[i] = std::make_shared<LODData>();
		lodDataList[i]->level = i;
	}

	// 레벨 0
	lodDataList[0]->indicesList.resize(1);

	for (DWORD dwRow = 0; dwRow < (DWORD)cellNum; dwRow++)
	{
		for (DWORD dwCol = 0; dwCol < (DWORD)cellNum; dwCol++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;

			lodDataList[0]->indicesList[0].push_back(dwRow * (cellNum + 1) + dwCol);
			lodDataList[0]->indicesList[0].push_back(dwRow * (cellNum + 1) + dwNextCol);
			lodDataList[0]->indicesList[0].push_back(dwNextRow * (cellNum + 1) + dwCol);
			lodDataList[0]->indicesList[0].push_back(dwNextRow * (cellNum + 1) + dwCol);
			lodDataList[0]->indicesList[0].push_back(dwRow * (cellNum + 1) + dwNextCol);
			lodDataList[0]->indicesList[0].push_back(dwNextRow * (cellNum + 1) + dwNextCol);
		}
	}
	CreateIndexBuffer(lodDataList[0]);

	// 레벨 1부터 ~  m_iPatchLodCount+1
	for (DWORD dwLevel = 0; dwLevel < (DWORD)patchLodCount; dwLevel++)
	{
		// 최소 패치의 크기가( 0 - 1 - 2 ) 정점 3개 이기 때문에 레벨=0 일 경우 오프셋=2이어야 한다.
		DWORD dwOffset = (DWORD)(pow(2.0f, float(dwLevel) + 1));
		DWORD dwNumCell = (cellNum / dwOffset);

		auto& indexList = lodDataList[dwLevel + 1]->indicesList;
		indexList.resize(16);

		for (int iData = 0; iData < 16; iData++)
		{
			DWORD dwIndexCounter = GetIndexCount(iData, dwNumCell);
			indexList[iData].resize(dwIndexCounter);
			int iIndex = 0;

			for (DWORD dwRow = 0; dwRow < (DWORD)cellNum; dwRow += dwOffset)
			{
				for (DWORD dwCol = 0; dwCol < (DWORD)cellNum; dwCol += dwOffset)
				{
					DWORD dwTL = dwRow * (cellNum + 1) + dwCol;
					DWORD dwTR = dwTL + dwOffset;
					DWORD dwBL = dwOffset * (cellNum + 1) + dwTL;
					DWORD dwBR = dwBL + dwOffset;
					DWORD dwCP = (dwTL + dwBR) / 2;

					if (iData != 0 &&
						(dwRow == 0 ||
							dwRow == ((dwNumCell - 1) * dwOffset) ||
							dwCol == 0 ||
							dwCol == ((dwNumCell - 1) * dwOffset))
						)
					{
						if ((iData & 1) && dwRow == 0)
						{
							DWORD dwUpperCenter = (dwTL + dwTR) / 2;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwTL;
							indexList[iData][iIndex++] = dwUpperCenter;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwUpperCenter;
							indexList[iData][iIndex++] = dwTR;
						}
						else
						{
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwTL;
							indexList[iData][iIndex++] = dwTR;
						}
						if ((iData & 2) && (dwCol == (dwNumCell - 1) * dwOffset))
						{
							DWORD dwRightCenter = (dwTR + dwBR) / 2;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwTR;
							indexList[iData][iIndex++] = dwRightCenter;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwRightCenter;
							indexList[iData][iIndex++] = dwBR;
						}
						else
						{
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwTR;
							indexList[iData][iIndex++] = dwBR;
						}
						if ((iData & 4) && (dwRow == (dwNumCell - 1) * dwOffset))
						{
							DWORD dwLowerCenter = (dwBR + dwBL) / 2;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwBR;
							indexList[iData][iIndex++] = dwLowerCenter;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwLowerCenter;
							indexList[iData][iIndex++] = dwBL;
						}
						else
						{
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwBR;
							indexList[iData][iIndex++] = dwBL;
						}
						if ((iData & 8) && (dwCol == 0))
						{
							DWORD dwLeftCenter = (dwTL + dwBL) / 2;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwBL;
							indexList[iData][iIndex++] = dwLeftCenter;
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwLeftCenter;
							indexList[iData][iIndex++] = dwTL;
						}
						else
						{
							indexList[iData][iIndex++] = dwCP;
							indexList[iData][iIndex++] = dwBL;
							indexList[iData][iIndex++] = dwTL;
						}
					}
					else
					{
						indexList[iData][iIndex++] = dwCP;
						indexList[iData][iIndex++] = dwTL;
						indexList[iData][iIndex++] = dwTR;

						indexList[iData][iIndex++] = dwCP;
						indexList[iData][iIndex++] = dwTR;
						indexList[iData][iIndex++] = dwBR;

						indexList[iData][iIndex++] = dwCP;
						indexList[iData][iIndex++] = dwBR;
						indexList[iData][iIndex++] = dwBL;

						indexList[iData][iIndex++] = dwCP;
						indexList[iData][iIndex++] = dwBL;
						indexList[iData][iIndex++] = dwTL;
					}
				}
			}

		}
		CreateIndexBuffer(lodDataList[dwLevel + 1]);
	}
}

void StaticLOD::CreateIndexBuffer(std::shared_ptr<LODData>& pNode)
{
	pNode->indexBufferList.resize(pNode->indicesList.size());

	for (int i = 0; i < pNode->indicesList.size(); ++i)
	{
		if (pNode->indicesList[i].size() < 0)
			break;

		pNode->indexBufferList[i] = std::make_shared<IndexBuffer>();
		pNode->indexBufferList[i]->CreateIndexBuffer(pNode->indicesList[i]);
	}
}

float StaticLOD::GetExpansionRatio(Vector3 center)
{
	auto cam = CameraManager::GetInstance().GetMainCamera();
	float farRange = cam->GetFarRange();

	float fDistance = (cam->position - center).Length();

	if (fDistance > farRange)
		return 1.0f;

	float fRatio = fDistance / farRange;

	return fRatio;
}

void StaticLOD::Release()
{
	for (auto& lodData : lodDataList)
	{
		lodData->indexBufferList.clear();
		lodData->indicesList.clear();
	}
	lodDataList.clear();
}
