#include "pch.h"
#include "SpaceDivideTree.h"

#include "SectionNode.h"
#include "Terrain.h"

#include "RenderMgr.h"
#include "Frustum.h"
#include "DebugDrawer.h"

// temp : for picking
void SpaceDivideTree::UpdateVertex()
{
    for (auto& node : leafNodeMap)
    {
        UpdateVertexList(node.second);
        CreateBoundingBox(node.second);
        node.second->UpdateVertexBuffer();
    }
}

SpaceDivideTree::SpaceDivideTree(std::shared_ptr<Terrain> owner) : terrain(owner)
{
}

SpaceDivideTree::~SpaceDivideTree()
{
}

// -------------------------------------------------------------------------------
// ------------------------------component functions -----------------------------
// -------------------------------------------------------------------------------

void SpaceDivideTree::Init()
{

	// temp : make shared resource 
	{
		shader = std::make_shared<Shader>(terrain.lock()->shaderFilePath);

		texture = std::make_shared<Texture>();
		texture->Load(terrain.lock()->textureFilePath);

        // temp : create render mgr
        renderMgr = std::make_shared<RenderMgr>();
        renderMgr->Init(shader);

        // temp : create frustum
        frustum = std::make_shared<Frustum>();

        // temp : for debug
        debugDraw = std::make_shared<DebugDrawer>();
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
}

void SpaceDivideTree::Update()
{
    //temp
    {
        // show mouse cursor point
        Vector2 mousePos = InputManager::GetInstance().GetMousePos();
        float mousePoses[2] = {mousePos.x, mousePos.y};
        ImGui::InputFloat2("Mouse Cursor Pos", &mousePoses[0]);
        //

        renderMgr->Update();
        frustum->Update();
        debugDraw->Update();

        if (InputManager::GetInstance().GetKeyState(DIK_P) == KeyState::PUSH)
        {
            //insert debug data to debugDraw
            for (int i = 0; i < drawNodeIndexList.size(); ++i)
            {
                auto& box = leafNodeMap[drawNodeIndexList[i]]->boundingBox;
                debugDraw->DrawBox(box, Vector4(1, 0, 0, 0));
            }

            // draw frustum
            {
                std::vector<Vector3> points1;
                std::vector<Vector3> points2;
                std::vector<Vector3> points3;
                std::vector<Vector3> points4;
                std::vector<Vector3> points5;
                std::vector<Vector3> points6;
                points1.push_back(frustum->frustumCorners[1]);
                points1.push_back(frustum->frustumCorners[2]);
                points1.push_back(frustum->frustumCorners[0]);
                points1.push_back(frustum->frustumCorners[3]);

                points2.push_back(frustum->frustumCorners[5]);
                points2.push_back(frustum->frustumCorners[6]);
                points2.push_back(frustum->frustumCorners[4]);
                points2.push_back(frustum->frustumCorners[7]);

                points3.push_back(frustum->frustumCorners[5]);
                points3.push_back(frustum->frustumCorners[1]);
                points3.push_back(frustum->frustumCorners[4]);
                points3.push_back(frustum->frustumCorners[0]);

                points4.push_back(frustum->frustumCorners[2]);
                points4.push_back(frustum->frustumCorners[6]);
                points4.push_back(frustum->frustumCorners[3]);
                points4.push_back(frustum->frustumCorners[7]);

                points5.push_back(frustum->frustumCorners[5]);
                points5.push_back(frustum->frustumCorners[6]);
                points5.push_back(frustum->frustumCorners[1]);
                points5.push_back(frustum->frustumCorners[2]);

                points6.push_back(frustum->frustumCorners[0]);
                points6.push_back(frustum->frustumCorners[3]);
                points6.push_back(frustum->frustumCorners[4]);
                points6.push_back(frustum->frustumCorners[7]);

                debugDraw->DrawRect(points1, Vector4(0, 1, 0, 0));
                debugDraw->DrawRect(points2, Vector4(0, 1, 0, 0));
                debugDraw->DrawRect(points3, Vector4(0, 1, 0, 0));
                debugDraw->DrawRect(points4, Vector4(0, 1, 0, 0));
                debugDraw->DrawRect(points5, Vector4(0, 1, 0, 0));
                debugDraw->DrawRect(points6, Vector4(0, 1, 0, 0));
            }
        }
    }

	FindDrawNode();
}

void SpaceDivideTree::Render()
{
    shader->GetSRV("MapBaseTexture")->SetResource(texture->GetShaderResourceView().Get());

    shader->GetSRV("MapAlphaTexture")->SetResource(terrain.lock()->alphaTexture->GetShaderResourceView().Get());

    shader->GetSRV("Texture1")->SetResource(terrain.lock()->texture1->GetShaderResourceView().Get());
    shader->GetSRV("Texture2")->SetResource(terrain.lock()->texture2->GetShaderResourceView().Get());
    shader->GetSRV("Texture3")->SetResource(terrain.lock()->texture3->GetShaderResourceView().Get());
    shader->GetSRV("Texture4")->SetResource(terrain.lock()->texture4->GetShaderResourceView().Get());

	for (auto& index : drawNodeIndexList)
	{
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
    for (int i = 0; i < leafNodeMap.size(); ++i)
    {
        isDraw = true;

        for (int j = 0; j < 8; ++j)
        {
            if (leafNodeMap[i]->boundingBox.ToPlane(frustum->planes[j]) == CollisionPos::Behind)
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

void SpaceDivideTree::CreateBoundingBox(std::shared_ptr<SectionNode> pNode)
{
	auto& conerIndexList = pNode->cornerIndexList;

	DWORD dwV0 = conerIndexList[0];
	DWORD dwV1 = conerIndexList[3];

	Vector2 vHeight = GetHeightFromNode(pNode);

	Vector3 min = Vector3::Zero;
	Vector3 max = Vector3::Zero;

	min.x = terrain.lock()->vertices[dwV0].position.x;
	min.y = vHeight.y;
	min.z = terrain.lock()->vertices[dwV1].position.z;

	max.x = terrain.lock()->vertices[dwV1].position.x;
	max.y = vHeight.x;
	max.z = terrain.lock()->vertices[dwV0].position.z;

	pNode->boundingBox.SetCube(min, max);
}

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
        pNode->shader = shader;

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
    for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
    {
        for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
        {
            int iCurrentIndex = iRow * iNumCols + iCol;
            pNode->vertices[iIndex++] = terrain.lock()->vertices[iCurrentIndex];
        }
    }
}

Vector2 SpaceDivideTree::GetHeightFromNode(std::shared_ptr<SectionNode> pNode)
{
    DWORD dwTL = pNode->cornerIndexList[0];
    DWORD dwTR = pNode->cornerIndexList[1];
    DWORD dwBL = pNode->cornerIndexList[2];
    DWORD dwBR = pNode->cornerIndexList[3];

    auto map = terrain.lock();
    DWORD dwWidth = terrain.lock()->colNum;

    DWORD dwStartCol = dwTL % dwWidth;
    DWORD dwEndCol = dwTR % dwWidth;
    DWORD dwStartRow = dwTL / dwWidth;
    DWORD dwEndRow = dwBL / dwWidth;

    Vector2 vHeight;
    vHeight.x = -99999999.0f;
    vHeight.y = 99999999.0f;

    for (int dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
    {
        for (int dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++)
        {
            DWORD dwCurrent = dwRow * dwWidth + dwCol;
            if (map->vertices[dwCurrent].position.y > vHeight.x)
            {
                vHeight.x = map->vertices[dwCurrent].position.y;
            }
            if (map->vertices[dwCurrent].position.y < vHeight.y)
            {
                vHeight.y = map->vertices[dwCurrent].position.y;
            }
        }
    }
    return vHeight;
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
    std::shared_ptr<SectionNode> NewNode = std::make_shared<SectionNode>();

    NewNode->parentNode = pParent;
    if(pParent != nullptr)
        NewNode->depth = pParent->depth + 1;
    
    NewNode->cornerIndexList.resize(4);
    NewNode->cornerIndexList[0] = LT;
    NewNode->cornerIndexList[1] = RT;
    NewNode->cornerIndexList[2] = LB;
    NewNode->cornerIndexList[3] = RB;

    //set node index
    ldiv_t divVal = ldiv((long)LT, (long)terrain.lock()->colNum);
    NewNode->element.x = divVal.rem / (RT - LT);
    NewNode->element.y = divVal.quot / (RT - LT);

    UINT dwNumPatchCount = (UINT)pow(2.0f, (float)NewNode->depth);
    NewNode->nodeIndex = NewNode->element.y * dwNumPatchCount + NewNode->element.x;

    CreateBoundingBox(NewNode);

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
