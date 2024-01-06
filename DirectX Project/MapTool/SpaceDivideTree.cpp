#include "pch.h"
#include "SpaceDivideTree.h"

#include "SectionNode.h"
#include "Terrain.h"
#include "StaticLOD.h"
#include "ObjectManager.h"

#include "DebugDrawer.h"

SpaceDivideTree::SpaceDivideTree()
{
    staticLOD = std::make_shared<StaticLOD>();
    objectManager = std::make_shared<ObjectManager>();
}

SpaceDivideTree::~SpaceDivideTree()
{
}

// -------------------------------------------------------------------------------
// ------------------------------component functions -----------------------------
// -------------------------------------------------------------------------------

void SpaceDivideTree::Init(std::shared_ptr<Terrain> owner)
{
	{
        debugDraw = std::make_shared<DebugDrawer>();
	}

    terrain = owner;
    maxDepth = terrain.lock()->devideTreeDepth;

	root = CreateNode(nullptr, 0, owner->rowCellNum
        , owner->rowNum * owner->colCellNum
        , owner->rowNum * owner->colNum - 1);
	BuildTree(root);

    // set neighbor node
    SetNeighborNode();

    // lod
    staticLOD->SetLod(owner->rowNum, maxDepth);


    // object manager
	objectManager->Init(owner->sceneFilePath);
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
            for (auto& idx : drawNodeIndexList)
            {
                auto& box = leafNodeMap[idx];
                debugDraw->DrawBox(box->boundingBox, Vector4(1, 0, 0, 0));
            }
        }
    }
    
	FindDrawNode();

}


void SpaceDivideTree::Render()
{
	for (auto& index : drawNodeIndexList)
	{
        auto& node = leafNodeMap[index];

        node->indexBuffer = staticLOD->GetLodIndexBuffer(node);

		leafNodeMap[index]->Render();
	}


    // for debug
    {
        debugDraw->Render();
    }
}

// -------------------------------------------------------------------------------
// ------------------------------ tree functions -----------------------------
// -------------------------------------------------------------------------------

void SpaceDivideTree::ShowObjectUI()
{
    objectManager->ShowObjectUI();
}

void SpaceDivideTree::ObjectPicking(Ray& ray)
{
    objectManager->ObjectPicking(ray);
}

void SpaceDivideTree::ShowObjectPickingUI()
{
    auto obj = objectManager->ShowObjectPickingUI();

    if (obj == nullptr)
        return;

    // delete from scene
    for (auto& nodeIdx : obj->groupNodeIdxList)
		SceneManager::GetInstance().GetCurrentScene()->Remove(obj, nodeIdx);

    obj->groupNodeIdxList.clear();
    for (auto& leafNode : leafNodeMap)
    {
        if (Collision::CubeToCube(leafNode.second->boundingBox, obj->GetTransform()->GetBoundingBox()))
        {
            obj->groupNodeIdxList.push_back(leafNode.first);
            SceneManager::GetInstance().GetCurrentScene()->Add(obj, leafNode.first);
        }
    }
}

void SpaceDivideTree::SpawnObject(Vector3& spawnPoint)
{
    auto obj = objectManager->SpawnObject(spawnPoint);

    if(obj == nullptr)
		return;

    for (auto& leafNode : leafNodeMap)
    {
        if (Collision::CubeToCube(leafNode.second->boundingBox, obj->GetTransform()->GetBoundingBox()))
        {
            obj->groupNodeIdxList.push_back(leafNode.first);
            SceneManager::GetInstance().GetCurrentScene()->Add(obj, leafNode.first);
        }
    }
}

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

void SpaceDivideTree::SaveScene(std::wstring sceneFilePath)
{
    objectManager->Save(sceneFilePath);
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

    SceneManager::GetInstance().GetCurrentScene()->SetDrawNodeIdxList(drawNodeIndexList);
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

    // �ִ� ���� �ѵ��� �����Ѵ�.
    if (maxDepth <= pNode->depth)
    {
        pNode->isLeafNode = true;
        
        UpdateVertexList(pNode);

        pNode->SetVertexBuffer();
        pNode->SetBoundingBox();

        leafNodeMap.insert(std::make_pair(pNode->nodeIndex, pNode));

        return false;
    }

    // ���� = �����ڳ� + ���ø�ũ��
    // ���ϱ� = ���� + ( ���ø� ũ�� * ��ü�� ����ũ�� )

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
    //���ϴ� LowQuality ��ġ�� ����
    for (int iNode = 0; iNode < leafNodeMap.size(); iNode++)
    {
        auto iter = leafNodeMap.find(iNode);
        assert(iter != leafNodeMap.end());

        auto& pNode = iter->second;
        DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->depth);
        DWORD dwNeighborCol = 0;
        DWORD dwNeighborRow = 0;

        pNode->neighborNodeList.resize(4);

        if (pNode->element.y > 0)  //��
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y - 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);

            assert(iter != leafNodeMap.end());

            pNode->neighborNodeList[3] = iter->second;
        }
        if (pNode->element.y < dwNumPatchCount - 1) // ��
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y + 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->neighborNodeList[2] = iter->second;
        }
        if (pNode->element.x > 0) // ��
        {
            dwNeighborCol = pNode->element.x - 1;
            dwNeighborRow = pNode->element.y * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->neighborNodeList[1] = iter->second;
        }
        if (pNode->element.x < dwNumPatchCount - 1) // ��
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

    // �񱳰��� ���ٸ� ���� ����ũ�⸦ ��ȯ�Ѵ�. ��) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
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
