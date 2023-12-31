#include "pch.h"
#include "Converter_FBXSDK.h"

Converter_FBXSDK::Converter_FBXSDK()
{
	pFbxManager = FbxManager::Create();
	pFbxImporter = FbxImporter::Create(pFbxManager, "");
	pFbxScene = FbxScene::Create(pFbxManager, "");
}

Converter_FBXSDK::~Converter_FBXSDK()
{
}

void Converter_FBXSDK::WriteAnimationData(std::shared_ptr<asAnimation> animation, std::wstring filePath)
{
	auto path = std::filesystem::path(filePath);

	std::filesystem::create_directory(path.parent_path());

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(filePath, FileMode::Write);

	file->Write<std::string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<UINT>(animation->frameCount);

	file->Write<UINT>(animation->keyframes.size());

	for (std::shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<std::string>(keyframe->boneName);

		file->Write<UINT>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

void Converter_FBXSDK::LoadFBX(std::wstring fileName)
{
	std::string cFileName = Utils::WStringToString(fileName);
	assert(pFbxImporter->Initialize(cFileName.c_str()));
	assert(pFbxImporter->Import(pFbxScene));

	pFbxRootNode = pFbxScene->GetRootNode();
	if (pFbxRootNode)
		PreProcess(pFbxRootNode);
}

void Converter_FBXSDK::PreProcess(FbxNode* fbxNode)
{
	if (fbxNode == nullptr)
		return;
	if (fbxNode->GetCamera() || fbxNode->GetLight())
		return;

	FbxMesh* pMesh = fbxNode->GetMesh();

	if (pMesh != nullptr)
		fbxNodeList.insert(std::make_pair( fbxNode->GetName(), fbxNode  ));
	

	UINT childCount = fbxNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)
		PreProcess(fbxNode->GetChild(i));
}

Matrix Converter_FBXSDK::ConvertAMatrix(FbxAMatrix& fbxMatrix)
{
	Matrix mat;

	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&fbxMatrix);

	for (int i = 0; i < 16; i++)
		pMatArray[i] = pSrcArray[i];

	return mat;
}

Matrix Converter_FBXSDK::ConvertDXMatrix(Matrix& dxMatrix)
{
	Matrix mat;

	mat._11 = dxMatrix._11; mat._12 = dxMatrix._13; mat._13 = dxMatrix._12;
	mat._21 = dxMatrix._31; mat._22 = dxMatrix._33; mat._23 = dxMatrix._32;
	mat._31 = dxMatrix._21; mat._32 = dxMatrix._23; mat._33 = dxMatrix._22;
	mat._41 = dxMatrix._41; mat._42 = dxMatrix._43; mat._43 = dxMatrix._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;

	return mat;
}

void Converter_FBXSDK::GetAnimationData(std::shared_ptr<asAnimation>& animation)
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = pFbxScene->GetSrcObject<FbxAnimStack>(0); // collection of animation layers

	if (stack == nullptr)
		return;

	FbxString stackName = stack->GetName();
	animation->name = stackName;

	FbxTakeInfo* takeInfo = pFbxScene->GetTakeInfo(stackName); // group of animation data grouped by name
	FbxTimeSpan timeSpan = takeInfo->mLocalTimeSpan; // time span of animation
	FbxTime startTime = timeSpan.GetStart();
	FbxTime endTime = timeSpan.GetStop();

	FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong frameStart = startTime.GetFrameCount(timeMode);
	FbxLongLong frameEnd = endTime.GetFrameCount(timeMode);

	animation->frameCount = frameEnd - frameStart;
	animation->frameRate = 30; // eframes30
	animation->duration = animation->frameCount / animation->frameRate;

	FbxTime frameTime;
	for(auto& node : fbxNodeList)
	{
		std::shared_ptr<asKeyframe> keyframe = std::make_shared<asKeyframe>();
		keyframe->boneName = node.first;

		for (int i = frameStart; i < frameEnd; ++i)
		{
			asKeyframeData frameData;
			frameData.time = (float)i;
			frameTime.SetFrame(i, timeMode);

			FbxAMatrix matFbx = node.second->EvaluateGlobalTransform(frameTime);
			//FbxAMatrix matFbx = fbxNodeList[node]->EvaluateLocalTransform(frameTime);
			Matrix mat = ConvertAMatrix(matFbx);
			mat = ConvertDXMatrix(mat);
			mat.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);

			keyframe->transforms.push_back(frameData);
		}

		animation->keyframes.push_back(keyframe);
	}
}

void Converter_FBXSDK::ReadAssetFile(const std::wstring& file)
{
	std::wstring fullPath = assetPath + file;

	auto p = std::filesystem::path(fullPath);

	if (!std::filesystem::exists(p))
		Utils::ShowErrorMessage(L"File not found: " + fullPath);

	LoadFBX(fullPath);
}

void Converter_FBXSDK::ExportAnimationData(const std::wstring& savePath, UINT index)
{
	std::wstring fullPath = modelPath + savePath + L".anim";

	std::shared_ptr<asAnimation> animation = std::make_shared<asAnimation>(); 
	GetAnimationData(animation);

	WriteAnimationData(animation, fullPath);
}

Matrix Converter_FBXSDK::GetGeometrMatrix(std::string nodeName)
{
	FbxNode* node = fbxNodeList[nodeName];

	if (node == nullptr)
		return Matrix();

	FbxAMatrix geometircMat;
	geometircMat.SetT(node->GetGeometricTranslation(FbxNode::eSourcePivot));
	geometircMat.SetR(node->GetGeometricRotation(FbxNode::eSourcePivot));
	geometircMat.SetS(node->GetGeometricScaling(FbxNode::eSourcePivot));

	Matrix result = ConvertAMatrix(geometircMat);

	return result;
}
