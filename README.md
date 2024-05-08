# DX_MapTool

## Developer Info
* 이름 : 유원석(You Won Sock)
* GitHub : https://github.com/youwonsock
* Mail : qazwsx233434@gmail.com

## Youtube

<b><h2>Platforms</h2></b>

<p>
<img src="https://upload.wikimedia.org/wikipedia/commons/c/c7/Windows_logo_-_2012.png" height="30">
</p>

### Development kits

<p>
<img src="https://upload.wikimedia.org/wikipedia/commons/7/7f/Microsoft-DirectX-Logo-wordmark.svg" height="80">
</p>
<p>
  ![스크린샷 2024-05-08 024741](https://github.com/youwonsock/DX_Maptool/assets/46276141/c4935dbe-ec4b-4aa6-a079-8dc77cd4f5f2)
</p>
<p>
  ![스크린샷 2024-05-08 024549](https://github.com/youwonsock/DX_Maptool/assets/46276141/5c1f9e63-0ee1-4612-9689-87cff0ba923d)
</p>

<b><h2>Periods</h2></b>

* 2023-12 - 2024-02

## Portfolio

### QuadTree 공간분할 Frustum Culling
  <details>
  <summary>QuadTree Code</summary>
  <div markdown="1">
    
  ```cpp

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

  ```
  </div>
  </details>
  
  <details>
  <summary>Frustum Culling Code</summary>
  <div markdown="1">
    
  ```cpp
    
    bool Collision::CubeToPlane(const Cube& cube, const Plane& plane)
    {
    	float fDist = 0.0f;
    	float fPlaneToCenter = 0.0f;
    	Vector3 vDir;
    
    	Vector3 axisVec[3];
    	axisVec[0] = cube.axisVector[0]; axisVec[0].Normalize();
    	axisVec[1] = cube.axisVector[1]; axisVec[1].Normalize();
    	axisVec[2] = cube.axisVector[2]; axisVec[2].Normalize();
    
    	vDir = axisVec[0] * (cube.size.x * 0.5f);
    	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
    	vDir = axisVec[1] * (cube.size.y * 0.5f);
    	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
    	vDir = axisVec[2] * (cube.size.z * 0.5f);
    	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
    
    	fPlaneToCenter = plane.normal.x * cube.center.x + plane.normal.y * cube.center.y 
    						+ plane.normal.z * cube.center.z + plane.normal.w;
    
    	if (fPlaneToCenter < -fDist)
    		return false;
    
    	return true;
    }

  ```
  </div>
  </details>
  QuadTree로 분할된 지형의 각 Node에 Obb 박스를 할당한 뒤 Plane과 Obb 충돌 연산을 통해 Draw할 Node를 선별합니다. 
  </br></br>



### 마우스 피킹 및 HeightMap 높이 조절, Texture Splatting
![스크린샷 2024-05-08 221326](https://github.com/youwonsock/DX_Maptool/assets/46276141/286c92c6-c4c7-4896-8f9e-10062d9e1a43)
![스크린샷 2024-05-08 221134](https://github.com/youwonsock/DX_Maptool/assets/46276141/079d9366-5c51-4b7f-a2d3-9f377b62cf41)
Ray와 Node의 OBB 박스와 1차 검사 후 충돌한 Node의 Face와 검사해서 충돌한 Face를 찾은 뒤 HeightMap, Splatting을 수행합니다.



### LOD
![스크린샷 2024-05-08 234205](https://github.com/youwonsock/DX_Maptool/assets/46276141/8a2b56b9-affb-435a-a364-dd377329dd66)
미리 레벨 별 IndexBuffer를 생성한 뒤 메인 카메라와의 거리에 따라 해당하는 IndexBuffer로 교체하여 Draw Call을 합니다. 



### Animation, Instansing
![스크린샷 2024-05-07 052204](https://github.com/youwonsock/DX_Maptool/assets/46276141/760739fd-58e9-43d2-8e7b-c13f05e6d444)
Texture에 최종 애니메이션 행렬을 저장 후 Shader에서 input데이터를 이용해 계산하는 방식으로 Skinning animation을 구현하였습니다.

![스크린샷 2024-05-07 052413](https://github.com/youwonsock/DX_Maptool/assets/46276141/7b012611-386c-45b6-b570-06c5cfd4abc2)
모든 Object를 vector에 저장 뒤 같은 모델을 사용하는 인스턴스 별로 다시 나눠서 Draw합니다. 



### Normal Mapping, Environment Mapping
![스크린샷 2024-05-08 223249](https://github.com/youwonsock/DX_Maptool/assets/46276141/55003ea8-8d95-4903-a2ce-88db5de5e210)
라이트 백터를 Tangent Space로 변환시킨 뒤 이미지에서 추출한 TBN을 이용해 세이더에서 처리합니다.

![스크린샷 2024-05-08 223921](https://github.com/youwonsock/DX_Maptool/assets/46276141/6789476e-6a57-4b2f-be32-18e1b7f931df)


### Depthmap Shadow
![스크린샷 2024-05-08 232038](https://github.com/youwonsock/DX_Maptool/assets/46276141/88fbd932-9cb8-453e-b5c0-a52856288f08)

### Save, Load

  <details>
  <summary>Frustum Save,Load Code</summary>
  <div markdown="1">
    
  ```cpp
    
    void Terrain::SaveMapData(std::wstring mapDataPath)
    {
    	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
    	file->Open(mapDataPath, FileMode::Write);
    
    	file->Write<std::string>(Utils::WStringToString(baseTexturePath));
    	file->Write<std::string>(Utils::WStringToString(splattingDataPath));
    	file->Write<std::string>(Utils::WStringToString(heightMapFilePath));
    	file->Write<std::string>(Utils::WStringToString(sceneFilePath));
    
    	file->Write<float>(heightScale);
    	file->Write<UINT>(rowNum);
    	file->Write<UINT>(colNum);
    	file->Write<int>(devideTreeDepth);
    	file->Write<float>(cellDistance);
    
    	heightMap->SaveHeightMap(heightMapFilePath);
    
    	PathString alphaPath(heightMapFilePath.c_str());
    	std::wstring alphaDrive = alphaPath.Drive;
    	std::wstring alphaDir = alphaPath.Dir;
    	std::wstring alphaFileName = alphaPath.Filename;
    	std::wstring alphaTexPath = alphaDrive + alphaDir + alphaFileName + L"_alpha.png";
    
    	splatting->Save(splattingDataPath, alphaTexPath);
    
    	spaceDivideTree->SaveScene(sceneFilePath);
    }
    
    void Terrain::LoadMapData(std::wstring mapDataPath)
    {
    	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
    
    	heightScale = mapDataDesc.heightScale;
    	this->rowNum = mapDataDesc.rowNum;
    	this->colNum = mapDataDesc.colNum;
    	devideTreeDepth = mapDataDesc.devideTreeDepth;
    	cellDistance = mapDataDesc.cellDistance;
    
    	baseTexturePath = L"../../Res/MapData/grass.jpg";
    	splattingDataPath = L"../../Res/MapData/splattingData" + std::to_wstring(rowNum) + L".splattingData";
    	heightMapFilePath = L"../../Res/MapData/heightMapTexture" + std::to_wstring(rowNum) + L".png";
    	sceneFilePath = L"../../Res/MapData/sceneFile" + std::to_wstring(rowNum) + L".sceneData";
    
    	if (file->Open(mapDataPath, FileMode::Read))
    	{
    		std::string baseTexture = file->Read<std::string>();
    		baseTexturePath = Utils::StringToWString(baseTexture);
    
    		std::string splattingData = file->Read<std::string>();
    		splattingDataPath = Utils::StringToWString(splattingData);
    
    		std::string heightMap = file->Read<std::string>();
    		heightMapFilePath = Utils::StringToWString(heightMap);
    
    		std::string scene = file->Read<std::string>();
    		sceneFilePath = Utils::StringToWString(scene);
    
    		heightScale = file->Read<float>();
    		rowNum = file->Read<UINT>();
    		colNum = file->Read<UINT>();
    		devideTreeDepth = file->Read<int>();
    		cellDistance = file->Read<float>();
    	}
    
    	// set resource, make picking, map renderer
    	{
    		shader = ResourceManager::GetInstance().Load<Shader>(L"MapToolShader", L"Shader/MapToolShader/MapToolShader.fx");
    
    		texture = ResourceManager::GetInstance().Load<Texture>(L"MapToolTexture", baseTexturePath, false);
    		shader->GetSRV("DiffuseMap")->SetResource(texture->GetShaderResourceView().Get());
    	}
    
    	// height map
    	{
    		heightMap->Init(rowNum, colNum, heightScale, heightMapFilePath);
    
    		// after height map create rowNum, colNum is power of 2 + 1
    		rowCellNum = rowNum - 1;
    		colCellNum = colNum - 1;
    		vertexCount = rowNum * colNum;
    		faceCount = rowCellNum * colCellNum * 2;
    	}
    
    	// splatting
    	{
    		splatting->Init(splattingDataPath, rowNum, colNum);
    	}
    
    	CreateVertexData();
    	CreateIndexData();
    
    	// GenerateVertexNormal, make normal table;
    	InitFaceNormal();
    	GenNormalLookupTable();
    	CalcPerVertexNormalsFastLookup();
    
    	// quad tree
    	spaceDivideTree->Init(shared_from_this());
    
    	// init color
    	splatting->SetVertexByTexture(vertices);
    
    	// create leaf node index list(for picking)
    	CreateLeafNodeIndexList();
    
    	// init map renderer
    	mapRenderer->Init();
    }

  ```
  </div>
  </details>
  Qua
  </br></br>
