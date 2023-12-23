#include "pch.h"
#include "SpaceDivideTree.h"

#include "SectionNode.h"
#include "Terrain.h"
#include "StaticLOD.h"

#include "RenderMgr.h"
#include "DebugDrawer.h"

void SpaceDivideTree::UpdateVertex(std::vector<SHORT> updateNodeIdxList)
{
    for (auto& idx : updateNodeIdxList)
    {
        auto& node = leafNodeMap[idx];

        UpdateVertexList(node);
        node->SetBoundingBox();
        node->UpdateVertexBuffer();
    }
}

SpaceDivideTree::SpaceDivideTree(std::shared_ptr<Terrain> owner) : terrain(owner)
{
}

SpaceDivideTree::~SpaceDivideTree()
{
}


// -------------------------------------------------------------------------------
// ------------------------------lod functions -----------------------------------
// -------------------------------------------------------------------------------

std::vector<DWORD> indexList;
std::map<int, std::shared_ptr<IndexBuffer>> lodIndexBufferMap;
int pacthCount = 0;

int a = 0;

float GetExpansionRatio(Vector3 center)
{
    auto mainCam = CameraManager::GetInstance().GetMainCamera();

    float a = Vector3(mainCam->position - center).Length();

    float ratio = Vector3(mainCam->position - center).Length() / mainCam->GetFarRange();
    //float ratio = Vector3(mainCam->position - center).Length() / 150;

    // lod 테스트를 위해 임시로 5배로 늘림
    ratio *= 5;

    return ratio;
}

DWORD GetLodSubIndex(std::shared_ptr<SectionNode>& pNode)
{
    pNode->lodLevel = GetExpansionRatio(pNode->boundingBox.center) * pacthCount;

    return 1;
}

DWORD GetLodType(std::shared_ptr<SectionNode>& pNode)
{
    if (pNode->neighborNodeList.size() <= 0)
        return 0;

    DWORD dwType = 0;

    // 상단
    if (pNode->neighborNodeList[0] && pNode->neighborNodeList[0]->lodLevel < pNode->lodLevel)
        dwType += 1;
    // 하단
    if (pNode->neighborNodeList[1] && pNode->neighborNodeList[1]->lodLevel < pNode->lodLevel)
        dwType += 4;
    // 좌측
    if (pNode->neighborNodeList[2] && pNode->neighborNodeList[2]->lodLevel < pNode->lodLevel)
        dwType += 8;
    // 우측
    if (pNode->neighborNodeList[3] && pNode->neighborNodeList[3]->lodLevel < pNode->lodLevel)
        dwType += 2;

    pNode->lodType = dwType;

    return dwType;
}

UINT beforePos = 0;
int SetLodIndexBuffer(std::shared_ptr<SectionNode>& pNode, DWORD& dwCurentIndex, DWORD dwA, DWORD dwB, DWORD dwC, DWORD dwType)
{
    int iNumFaces = 0;

    if (dwType == 0)
    {
        indexList[dwCurentIndex + 0] = dwA;
        indexList[dwCurentIndex + 1] = dwB;
        indexList[dwCurentIndex + 2] = dwC;
        iNumFaces += 1;
        dwCurentIndex += 3;
    }
    else if (dwType == 8 || dwType == 2)// 좌우
    {
        DWORD dwCenter = (dwA + dwB) / 2;
        indexList[dwCurentIndex + 0] = dwC;
        indexList[dwCurentIndex + 1] = dwA;
        indexList[dwCurentIndex + 2] = dwCenter;
        indexList[dwCurentIndex + 3] = dwC;
        indexList[dwCurentIndex + 4] = dwCenter;
        indexList[dwCurentIndex + 5] = dwB;
        iNumFaces += 2;
        dwCurentIndex += 6;

    }
    else if (dwType == 1 || dwType == 4)// 상하
    {
        DWORD dwCenter = (dwB + dwC) / 2;
        indexList[dwCurentIndex + 0] = dwA;
        indexList[dwCurentIndex + 1] = dwB;
        indexList[dwCurentIndex + 2] = dwCenter;
        indexList[dwCurentIndex + 3] = dwA;
        indexList[dwCurentIndex + 4] = dwCenter;
        indexList[dwCurentIndex + 5] = dwC;
        iNumFaces += 2;
        dwCurentIndex += 6;
    }
    else if (dwType == 9 || dwType == 6)// 좌상, 우하
    {
        DWORD dwTopCenter = (dwB + dwC) / 2;
        DWORD dwLeftCenter = (dwA + dwB) / 2;

        indexList[dwCurentIndex + 0] = dwLeftCenter;
        indexList[dwCurentIndex + 1] = dwB;
        indexList[dwCurentIndex + 2] = dwTopCenter;
        indexList[dwCurentIndex + 3] = dwLeftCenter;
        indexList[dwCurentIndex + 4] = dwTopCenter;
        indexList[dwCurentIndex + 5] = dwA;

        indexList[dwCurentIndex + 6] = dwA;
        indexList[dwCurentIndex + 7] = dwTopCenter;
        indexList[dwCurentIndex + 8] = dwC;
        iNumFaces += 3;
        dwCurentIndex += 9;
    }

    return iNumFaces;
}

int UpdateIndexList(std::shared_ptr<SectionNode>& pNode, DWORD idxBufferIdx, DWORD lodLevel, DWORD width)
{
    int iNumFaces = 0; beforePos = 0;

    DWORD dwTL = pNode->cornerIndexList[0];
    DWORD dwTR = pNode->cornerIndexList[1];
    DWORD dwBL = pNode->cornerIndexList[2];
    DWORD dwBR = pNode->cornerIndexList[3];

    /*ldiv_t divValue = ldiv( dwTL, m_dwWidth );
    DWORD dwStartRow = divValue.quot;
    DWORD dwStartCol = divValue.rem;

    divValue = ldiv( dwBL, m_dwWidth );
    DWORD dwEndRow = divValue.quot;

    divValue = ldiv( dwTR, m_dwWidth );
    DWORD dwEndCol = divValue.rem;*/

    DWORD dwStartRow = dwTL / width;
    DWORD dwEndRow = dwBL / width;

    DWORD dwStartCol = dwTL % width;
    DWORD dwEndCol = dwTR % width;

    DWORD dwOffset = (DWORD)(pow(2.0f, (float)lodLevel));
    DWORD dwCountX = ((dwEndCol - dwStartCol) / dwOffset) - 1;
    DWORD dwCountY = ((dwEndRow - dwStartRow) / dwOffset) - 1;


    DWORD dwYCell = 0;
    for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow += dwOffset, dwYCell++)
    {
        DWORD dwXCell = 0;
        for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol += dwOffset, dwXCell++)
        {
            //0	1    4   
            //2	   3 5
            DWORD dwNextRow = dwRow + dwOffset;
            DWORD dwNextCol = dwCol + dwOffset;
            if ((dwXCell == 0 && dwYCell == 0))
            {
                DWORD dwType = (pNode->lodType & 8) + (pNode->lodType & 1);
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwNextRow * width + dwCol,	// 2
                    dwRow * width + dwCol,		// 0
                    dwRow * width + dwNextCol,	// 1											
                    dwType);
            }
            else if ((dwXCell == 0) && (pNode->lodType & 8))
            {
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwNextRow * width + dwCol,	// 2
                    dwRow * width + dwCol,		// 0
                    dwRow * width + dwNextCol,	// 1											
                    8);
            }
            else if ((dwYCell == 0) && (pNode->lodType & 1))
            {
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwNextRow * width + dwCol,	// 2
                    dwRow * width + dwCol,		// 0
                    dwRow * width + dwNextCol,	// 1											
                    1);
            }
            else
            {
                indexList[idxBufferIdx + 0] = dwNextRow * width + dwCol;
                indexList[idxBufferIdx + 1] = dwRow * width + dwCol;
                indexList[idxBufferIdx + 2] = dwRow * width + dwNextCol;
                iNumFaces += 1;
                idxBufferIdx += 3;
            }



            if ((dwXCell == dwCountX && dwYCell == dwCountY))
            {
                DWORD dwType = (pNode->lodType & 2) + (pNode->lodType & 4);
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwRow * width + dwNextCol,		// 2
                    dwNextRow * width + dwNextCol,	// 3
                    dwNextRow * width + dwCol,		// 0																					
                    dwType);
            }
            else if ((dwXCell == dwCountX) && (pNode->lodType & 2))
            {
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwRow * width + dwNextCol,		// 2
                    dwNextRow * width + dwNextCol,	// 3
                    dwNextRow * width + dwCol,		// 0																					
                    2);
            }
            else if ((dwYCell == dwCountY) && (pNode->lodType & 4))
            {
                iNumFaces += SetLodIndexBuffer(pNode, idxBufferIdx,
                    dwRow * width + dwNextCol,		// 2
                    dwNextRow * width + dwNextCol,	// 3
                    dwNextRow * width + dwCol,		// 0																					
                    4);
            }
            else
            {
                indexList[idxBufferIdx + 0] = dwRow * width + dwNextCol;
                indexList[idxBufferIdx + 1] = dwNextRow * width + dwNextCol;
                indexList[idxBufferIdx + 2] = dwNextRow * width + dwCol;
                iNumFaces += 1;
                idxBufferIdx += 3;
            }
        }
    }

    std::shared_ptr<IndexBuffer> lodIndexBuffer = std::make_shared<IndexBuffer>();
    std::vector<UINT> indexListTemp;

    // copy current index list
    for (int i = beforePos; i < beforePos + idxBufferIdx; ++i)
        indexListTemp.push_back(indexList[i]);
    beforePos = idxBufferIdx;

    lodIndexBuffer->CreateIndexBuffer(indexListTemp);

    lodIndexBufferMap[pNode->nodeIndex] = lodIndexBuffer;

    return iNumFaces;
}

// -------------------------------------------------------------------------------
// ------------------------------lod functions -----------------------------------
// -------------------------------------------------------------------------------



// -------------------------------------------------------------------------------
// ------------------------------component functions -----------------------------
// -------------------------------------------------------------------------------

void SpaceDivideTree::Init()
{
	{
        debugDraw = std::make_shared<DebugDrawer>();
	}

    // temp lod
    {
        indexList.resize(terrain.lock()->rowNum * terrain.lock()->colNum * 2 * 3 * 2);
    }

	root = CreateNode(nullptr, 0, terrain.lock()->rowCellNum
        , terrain.lock()->rowNum * terrain.lock()->colCellNum
        , terrain.lock()->rowNum * terrain.lock()->colNum - 1);
	BuildTree(root);

    // set neighbor node
    SetNeighborNode();

    auto box = leafNodeMap[0]->boundingBox;
    UINT leafNodeRowNum = box.max.x - box.min.x;
    UINT leafNodeColNum = box.max.z - box.min.z;
    CreateIndexBuffer(leafNodeRowNum, leafNodeColNum);


    //temp test lod1 (use all vertex)
    {
        vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->CreateVertexBuffer(terrain.lock()->vertices);

        // set patch count
        int cellCount = (terrain.lock()->rowCellNum) / pow(2.0f, (float)maxDepth);
        pacthCount = ( log( (float)cellCount / log(2.0f) ) );
    }
}

void SpaceDivideTree::Update()
{
    debugDraw->Update();

    //temp
    if (InputManager::GetInstance().GetKeyState(DIK_P) == KeyState::PUSH)
    {
        // draw frustum
        {
            auto& frustum = CameraManager::GetInstance().GetMainCamera()->GetFrustum();
    
            std::vector<Vector3> points1;
            std::vector<Vector3> points2;
            std::vector<Vector3> points3;
            std::vector<Vector3> points4;
            std::vector<Vector3> points5;
            std::vector<Vector3> points6;
            points1.push_back(frustum.frustumCorners[1]);
            points1.push_back(frustum.frustumCorners[2]);
            points1.push_back(frustum.frustumCorners[0]);
            points1.push_back(frustum.frustumCorners[3]);
    
            points2.push_back(frustum.frustumCorners[5]);
            points2.push_back(frustum.frustumCorners[6]);
            points2.push_back(frustum.frustumCorners[4]);
            points2.push_back(frustum.frustumCorners[7]);
    
            points3.push_back(frustum.frustumCorners[5]);
            points3.push_back(frustum.frustumCorners[1]);
            points3.push_back(frustum.frustumCorners[4]);
            points3.push_back(frustum.frustumCorners[0]);
    
            points4.push_back(frustum.frustumCorners[2]);
            points4.push_back(frustum.frustumCorners[6]);
            points4.push_back(frustum.frustumCorners[3]);
            points4.push_back(frustum.frustumCorners[7]);
    
            points5.push_back(frustum.frustumCorners[5]);
            points5.push_back(frustum.frustumCorners[6]);
            points5.push_back(frustum.frustumCorners[1]);
            points5.push_back(frustum.frustumCorners[2]);
    
            points6.push_back(frustum.frustumCorners[0]);
            points6.push_back(frustum.frustumCorners[3]);
            points6.push_back(frustum.frustumCorners[4]);
            points6.push_back(frustum.frustumCorners[7]);
    
            debugDraw->DrawRect(points1, Vector4(0, 1, 0, 0));
            debugDraw->DrawRect(points2, Vector4(0, 1, 0, 0));
            debugDraw->DrawRect(points3, Vector4(0, 1, 0, 0));
            debugDraw->DrawRect(points4, Vector4(0, 1, 0, 0));
            debugDraw->DrawRect(points5, Vector4(0, 1, 0, 0));
            debugDraw->DrawRect(points6, Vector4(0, 1, 0, 0));
        }

        // draw box
        {
            for(auto& box : leafNodeMap)
				debugDraw->DrawBox(box.second->boundingBox, Vector4(1, 0, 0, 0));
        }
    }
    
	FindDrawNode();



    // update lod indexlist
    {
        numFace = 0;

        for (auto& i : drawNodeIndexList)
        {
            auto& node = leafNodeMap[i]; 
            
            GetLodSubIndex(node);
            GetLodType(node);

            numFace += UpdateIndexList(node, numFace * 3, node->lodLevel, terrain.lock()->rowNum); 
        }
    }
}


void SpaceDivideTree::Render()
{

    //test use all vertex buffer
    {
        UINT stride = vertexBuffer->GetStride();
        UINT offset = vertexBuffer->GetOffset();

        Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
    }

	for (auto& index : drawNodeIndexList)
	{
        auto& node = leafNodeMap[index];

        node->indexBuffer = lodIndexBufferMap[index];

		leafNodeMap[index]->Render();
	}


    // for debug
    {
        debugDraw->Render();
    }
}

void SpaceDivideTree::FindDrawNode()
{
    drawNodeIndexList.clear();

    bool isDraw = false;
    auto& frustum = CameraManager::GetInstance().GetMainCamera()->GetFrustum();

    for (int i = 0; i < leafNodeMap.size(); ++i)
    {
        isDraw = true;

        for (int j = 0; j < 6; ++j)
        {
            if (!Collision::CubeToPlane(leafNodeMap[i]->boundingBox, frustum.planes[j]))
            {
                isDraw = false;
                break;
            }
        }

        if(isDraw)
			drawNodeIndexList.push_back(leafNodeMap[i]->nodeIndex);
	}
}


// -------------------------------------------------------------------------------
// ------------------------------build tree functions ----------------------------
// -------------------------------------------------------------------------------

void SpaceDivideTree::BuildTree(std::shared_ptr<SectionNode> pNode)
{
    if (SubDivide(pNode))
    {
        for (int iNode = 0; iNode < 4; ++iNode)
            BuildTree(pNode->childNodeList[iNode]);
    }
}

bool SpaceDivideTree::SubDivide(std::shared_ptr<SectionNode> pNode)
{
    if (pNode == nullptr) return false;

    auto& conerIndexList = pNode->cornerIndexList;
    UINT width = terrain.lock()->rowNum;

    // 최대 깊이 한도로 제한한다.
    if (maxDepth <= pNode->depth)
    {
        pNode->isLeafNode = true;
        
        UpdateVertexList(pNode);

        pNode->SetVertexBuffer();
        pNode->SetBoundingBox();
        pNode->shader = terrain.lock()->shader;

        leafNodeMap.insert(std::make_pair(pNode->nodeIndex, pNode));

        return false;
    }

    // 우상귀 = 좌측코너 + 스플릿크기
    // 우하귀 = 우상귀 + ( 스플릿 크기 * 전체맵 가로크기 )

    UINT dwOffsetX = (conerIndexList[1] - conerIndexList[0]);
    UINT dwOffsetZ = ((conerIndexList[2] - conerIndexList[0]) / width);

	UINT dwWidthSplit = CheckSize(dwOffsetX);
	UINT dwHeightplit = CheckSize(dwOffsetZ);
    UINT dwSplitCenter = (conerIndexList[0] + dwWidthSplit) + (dwHeightplit * width);

    UINT dwEdgeCenter[4];
    dwEdgeCenter[0] = conerIndexList[0] + dwWidthSplit;
    dwEdgeCenter[1] = conerIndexList[1] + dwHeightplit * width;
    dwEdgeCenter[2] = conerIndexList[2] + dwWidthSplit;
    dwEdgeCenter[3] = conerIndexList[0] + dwHeightplit * width;

    pNode->childNodeList.push_back(CreateNode(pNode, conerIndexList[0],
        dwEdgeCenter[0],
        dwEdgeCenter[3],
        dwSplitCenter));

    pNode->childNodeList.push_back(CreateNode(pNode, dwEdgeCenter[0],
		conerIndexList[1],
        dwSplitCenter,
        dwEdgeCenter[1]));

    pNode->childNodeList.push_back(CreateNode(pNode, dwEdgeCenter[3],
        dwSplitCenter,
		conerIndexList[2],
        dwEdgeCenter[2]));

    pNode->childNodeList.push_back(CreateNode(pNode, dwSplitCenter,
        dwEdgeCenter[1],
        dwEdgeCenter[2],
		conerIndexList[3]));

    return true;
}

void SpaceDivideTree::SetNeighborNode()
{
    //최하단 LowQuality 패치의 기준
    for (int iNode = 0; iNode < leafNodeMap.size(); iNode++)
    {
        auto iter = leafNodeMap.find(iNode);
        assert(iter != leafNodeMap.end());

        auto& pNode = iter->second;
        DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->depth);
        DWORD dwNeighborCol = 0;
        DWORD dwNeighborRow = 0;

        pNode->neighborNodeList.resize(4);

        if (pNode->element.y > 0)  //상
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y - 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);

            assert(iter != leafNodeMap.end());

            pNode->neighborNodeList[3] = iter->second;
        }
        if (pNode->element.y < dwNumPatchCount - 1) // 하
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y + 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->neighborNodeList[2] = iter->second;
        }
        if (pNode->element.x > 0) // 좌
        {
            dwNeighborCol = pNode->element.x - 1;
            dwNeighborRow = pNode->element.y * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->neighborNodeList[1] = iter->second;
        }
        if (pNode->element.x < dwNumPatchCount - 1) // 우
        {
            dwNeighborCol = pNode->element.x + 1;
            dwNeighborRow = pNode->element.y * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->neighborNodeList[0] = iter->second;
        }
    }
}

void SpaceDivideTree::UpdateVertexList(std::shared_ptr<SectionNode> pNode)
{
    int iNumCols = terrain.lock()->colNum;
    int iStartRow = pNode->cornerIndexList[0] / iNumCols;
    int iEndRow = pNode->cornerIndexList[2] / iNumCols;
    int iStartCol = pNode->cornerIndexList[0] % iNumCols;
    int iEndCol = pNode->cornerIndexList[1] % iNumCols;

    int iNumColCell = iEndCol - iStartCol;
    int iNumRowCell = iEndRow - iStartRow;
    pNode->vertices.resize((iEndCol - iStartCol + 1) * (iEndRow - iStartRow + 1));

    int iIndex = 0;
    auto& terrainVertexList = terrain.lock()->vertices;
    for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
    {
        for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
        {
            int iCurrentIndex = iRow * iNumCols + iCol;
            pNode->vertices[iIndex++] = terrainVertexList[iCurrentIndex];
        }
    }
}

UINT SpaceDivideTree::CheckSize(UINT dwSize)
{
    float fCount = 0;
    DWORD dwChecker = dwSize / 2;
    if (dwChecker <= 0) return 0;

    while (dwChecker != 0)
    {
        dwChecker >>= 1;
        fCount += 1.0f;
    }
    float fMinimum = pow(2.0f, fCount - 1.0f);
    float fMaximum = pow(2.0f, fCount);
    // fMin = abs( fMin - P0 ) - abs( P1 - fMin );
    // fMax = abs( fMax - P0 ) - abs( P1 - fMax );
    // return min( fMin, fMax );
    float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
    float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

    // 비교값이 같다면 작은 분할크기를 반환한다. 예) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
    if (fMin <= fMax)
    {
        return (UINT)fMinimum;
    }
    return (UINT)fMaximum;
}

std::shared_ptr<SectionNode> SpaceDivideTree::CreateNode(std::shared_ptr<SectionNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB)
{
    SectionNodeDesc desc;

    desc.pParent = pParent;
    desc.LT = LT;
    desc.RT = RT;
    desc.LB = LB;
    desc.RB = RB;
    desc.colNum = terrain.lock()->colNum;

    std::shared_ptr<SectionNode> NewNode = std::make_shared<SectionNode>(desc);



    return NewNode;
}

void SpaceDivideTree::CreateIndexBuffer(UINT rowCellNum, UINT colCellNum)
{
    if (leafNodeIndexBuffer == nullptr)
    {
        leafNodeIndexList;

        UINT faceCount = rowCellNum * colCellNum * 2;
        leafNodeIndexList.resize(faceCount * 3);

        UINT rowNum = rowCellNum + 1;
        UINT colNum = colCellNum + 1;

        UINT iIndex = 0;
        for (UINT iRow = 0; iRow < rowCellNum; ++iRow)
        {
            for (UINT iCol = 0; iCol < colCellNum; ++iCol)
            {
				UINT nextCol = iCol + 1;
				UINT nextRow = iRow + 1;

                leafNodeIndexList[iIndex + 0] = iRow * colNum + iCol;
                leafNodeIndexList[iIndex + 1] = iRow * colNum + nextCol;
                leafNodeIndexList[iIndex + 2] = nextRow * colNum + iCol;

                leafNodeIndexList[iIndex + 3] = leafNodeIndexList[iIndex + 2];
                leafNodeIndexList[iIndex + 4] = leafNodeIndexList[iIndex + 1];
                leafNodeIndexList[iIndex + 5] = nextRow * colNum + nextCol;

				iIndex += 6;
			}
		}

        leafNodeIndexBuffer = std::make_shared<IndexBuffer>();
        leafNodeIndexBuffer->CreateIndexBuffer(leafNodeIndexList);
    }

    for (auto& node : leafNodeMap)
        node.second->indexBuffer = leafNodeIndexBuffer;
}
