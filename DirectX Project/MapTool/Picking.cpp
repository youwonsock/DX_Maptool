#include "pch.h"
#include "Picking.h"

#include "SectionNode.h"
#include "Terrain.h"

void Picking::FindChangeVertex(Vector3 centerPos, float findRange, int leafNodeNum
								, std::shared_ptr<SectionNode>& pickNode
								, std::vector<PNCTVertex>& vertexList)
{	
	// 0 : right, 1 : left, 2 : bottom, 3 : top
	// find node by node idx
	DWORD LT, RT, LB;
	DWORD leftTopNodeIdx, leftBottomNodeIdx, rightTopNodeIdx;

	std::shared_ptr<SectionNode> pNode = pickNode;

	// lt, lb
	{
		while (true)
		{
			if (pNode->neighborNodeList[1] == nullptr)
				break;

			auto& ltConerPos = vertexList[pNode->cornerIndexList[0]].position;

			if (centerPos.x - findRange < ltConerPos.x)
				pNode = pNode->neighborNodeList[1];
			else
				break;
		}

		// top
		std::shared_ptr<SectionNode> ltNode = pNode;

		while (true)
		{
			if (ltNode->neighborNodeList[3] == nullptr)
				break;

			auto& ltConerPos = vertexList[ltNode->cornerIndexList[0]].position;

			if (centerPos.z + findRange > ltConerPos.z)
				ltNode = ltNode->neighborNodeList[3];
			else
				break;
		}
		LT = ltNode->cornerIndexList[0];
		leftTopNodeIdx = ltNode->nodeIndex;

		// bottom
		std::shared_ptr<SectionNode> lbNode = pNode;

		while (true)
		{
			if (lbNode->neighborNodeList[2] == nullptr)
				break;

			auto& lbConerPos = vertexList[lbNode->cornerIndexList[2]].position;

			if (centerPos.z - findRange < lbConerPos.z)
				lbNode = lbNode->neighborNodeList[2];
			else
				break;
		}
		LB = lbNode->cornerIndexList[2];
		leftBottomNodeIdx = lbNode->nodeIndex;
	}


	// rt, rb
	{
		pNode = pickNode;

		while (true)
		{
			if (pNode->neighborNodeList[0] == nullptr)
				break;

			auto& rtConerPos = vertexList[pNode->cornerIndexList[1]].position;

			if (centerPos.x + findRange > rtConerPos.x)
				pNode = pNode->neighborNodeList[0];
			else
				break;
		}


		// top
		std::shared_ptr<SectionNode> rtNode = pNode;

		while (true)
		{
			if (rtNode->neighborNodeList[3] == nullptr)
				break;

			auto& rtConerPos = vertexList[rtNode->cornerIndexList[1]].position;

			if (centerPos.z + findRange > rtConerPos.z)
				rtNode = rtNode->neighborNodeList[3];
			else
				break;
		}
		RT = rtNode->cornerIndexList[1];
		rightTopNodeIdx = rtNode->nodeIndex;
	}

	// get update node idx
	{
		SHORT nodeColNum = sqrt(leafNodeNum);
		SHORT iStartRow = leftTopNodeIdx / nodeColNum;
		SHORT iEndRow = leftBottomNodeIdx / nodeColNum;
		SHORT iStartCol = leftTopNodeIdx % nodeColNum;
		SHORT iEndCol = rightTopNodeIdx % nodeColNum;

		SHORT iIndex = 0;
		UpdateNodeIdxList.clear();
		for (SHORT iRow = iStartRow; iRow <= iEndRow; iRow++)
		{
			for (SHORT iCol = iStartCol; iCol <= iEndCol; iCol++)
			{
				SHORT iCurrentIndex = iRow * nodeColNum + iCol;

				UpdateNodeIdxList.push_back(iCurrentIndex);
			}
		}
	}

	// get update vertex idx
	{
		Circle circle = Circle(Vector2(centerPos.x, centerPos.z), findRange);

		int iNumCols = sqrt(vertexList.size());
		int iStartRow = LT / iNumCols;
		int iEndRow = LB / iNumCols;
		int iStartCol = LT % iNumCols;
		int iEndCol = RT % iNumCols;

		int iIndex = 0;

		UpdateVertexIdxList.clear();
		for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
		{
			for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
			{
				int iCurrentIndex = iRow * iNumCols + iCol;

				if (Collision::CircleToPoint(circle, vertexList[iCurrentIndex].position))
					UpdateVertexIdxList.push_back(iCurrentIndex);
			}
		}
	}
}

std::shared_ptr<SectionNode>& Picking::FindPickFace(Ray& ray, std::vector<UINT>& leafNodeIdxList
														, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap
														, OUT Vector3& pickPoint)
{
	// find leaf node collision with ray
	std::vector<std::shared_ptr<SectionNode>> sectionList;
	for (int i = 0; i < leafNodeMap.size(); ++i)
	{
		if (Collision::CubeToRay(leafNodeMap[i]->boundingBox, ray))
			sectionList.push_back(leafNodeMap[i]);
	}

	// find face collision with ray
	for(int i = 0; i < sectionList.size(); ++i)
	{
		auto& leafVertices = sectionList[i]->vertices;
		for (int j = 0; j < leafNodeIdxList.size(); j += 3)
		{
			Vector3 v0 = leafVertices[leafNodeIdxList[j + 0]].position;
			Vector3 v1 = leafVertices[leafNodeIdxList[j + 1]].position;
			Vector3 v2 = leafVertices[leafNodeIdxList[j + 2]].position;

			if (Collision::RayToFace(ray, v0, v1, v2, &pickPoint))
				return leafNodeMap[i];
		}
	}

	std::shared_ptr<SectionNode> pickNode = nullptr;
	return pickNode;
}

