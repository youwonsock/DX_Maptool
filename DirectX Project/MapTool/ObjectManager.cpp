#include "pch.h"
#include "ObjectManager.h"

#include "SectionNode.h"
#include "AssimpTool\Converter.h"

#include "DebugDrawer.h"

void ObjectManager::Init(std::wstring sceneFilePath)
{
	shader = ResourceManager::GetInstance().Load<Shader>(L"ObjectShader", L"Shader/MapToolShader/ObjectShader.fx");
	objectCount = 0;
	ReadObjectModelNameList();

	//load
	Load(sceneFilePath);
}

void ObjectManager::ReadObjectModelNameList()
{
	objectModelNameList.clear();
	std::wstring path = L"../../Res/Models/";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		auto t = file.path().filename().string();
		objectModelNameList.push_back(t);
	}
}

void ObjectManager::SpawnObject(Vector3& spawnPoint)
{
	std::shared_ptr<GameObject> obj = nullptr;

	if(selectedModelIdx < 0)
		return;

	// load model
	std::wstring modelName = Utils::StringToWString(objectModelNameList[selectedModelIdx]);
	auto model = ResourceManager::GetInstance().Load<Model>(modelName, modelName + L"/" + modelName, false);

	if(model == nullptr)
		return;

	// make object
	obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<ModelRenderer>(shader));

	obj->GetModelRenderer()->SetModel(model);
	int pass = model->HasAnimation() ? 1 : 0;
	obj->GetModelRenderer()->SetPass(pass);

	obj->GetTransform()->SetWorldPosition(spawnPoint);
	objectList.push_back(obj);

	obj->objectName = modelName + std::to_wstring(objectCount++);

	SceneManager::GetInstance().GetCurrentScene()->Add(obj);
}

void ObjectManager::ObjectPicking(Ray& ray)
{
	for (auto& obj : objectList)
	{
		if (Collision::CubeToRay(obj->GetTransform()->GetBoundingBox(), ray))
		{
			pickObject = obj;
			
			selectedObjectIdx = std::find(objectList.begin(), objectList.end(), obj) - objectList.begin();

			return;
		}
	}
}

void ObjectManager::ShowObjectUI()
{
	ImGui::Text("Model List");
	if (ImGui::BeginListBox("Model List", ImVec2(-FLT_MIN, 25 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < objectModelNameList.size(); n++)
		{
			const bool is_selected = (selectedModelIdx == n);
			if (ImGui::Selectable( objectModelNameList[n].c_str(), is_selected))
				selectedModelIdx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}


	// open Dialog Simple`
	if (ImGui::Button("Load FBX File"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", "FBX files(*.fbx *.FBX){ .fbx, .FBX}", "../../Res/Assets/");


	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			std::wstring wFileName = Utils::StringToWString(fileName);

			Utils::Replace(wFileName, L".FBX", L"");
			Utils::Replace(wFileName, L".fbx", L"");
			// action

			//need assimp tool 
			std::shared_ptr<Converter> converter = std::make_shared<Converter>();

			converter->ReadAssetFile(wFileName + L"/" + wFileName + L".fbx");
			converter->ExportMaterialData(wFileName + L"/" + wFileName);
			converter->ExportModelData(wFileName + L"/" + wFileName);

			UseLib type = UseLib::FBXSDK;
			converter->ExportAnimationData(wFileName + L"/" + wFileName, 0, type);

			ReadObjectModelNameList();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void ObjectManager::Save(std::wstring sceneFilePath)
{
	std::map<std::wstring, std::vector<std::shared_ptr<GameObject>>> objectMap;

	for (auto& obj : objectList)
	{
		std::wstring modelName = obj->GetModelRenderer()->GetModel()->GetName();
		objectMap[modelName].push_back(obj);
	}

	auto path = std::filesystem::path(sceneFilePath);

	std::filesystem::create_directory(path.parent_path());

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	file->Open(sceneFilePath, FileMode::Write);

	file->Write<UINT>(objectCount);

	for (auto& object : objectMap)
	{
		file->Write<std::string>(Utils::WStringToString(object.first));
		file->Write<UINT>(object.second.size());

		for (auto& obj : object.second)
		{
			file->Write<std::string>(Utils::WStringToString(obj->objectName));

			file->Write<Vector3>(obj->GetTransform()->GetWorldPosition());

			Vector3 rot = obj->GetTransform()->GetWorldRotation();
			rot.x = XMConvertToDegrees(rot.x);
			rot.y = XMConvertToDegrees(rot.y);
			rot.z = XMConvertToDegrees(rot.z);
			file->Write<Vector3>(rot);

			file->Write<Vector3>(obj->GetTransform()->GetWorldScale());
		}
	}
}

void ObjectManager::Load(std::wstring sceneFilePath)
{
	objectList.clear();
	SceneManager::GetInstance().GetCurrentScene()->ClearScene();

	std::shared_ptr<FileUtils> file = std::make_shared<FileUtils>();
	if (!file->Open(sceneFilePath, FileMode::Read))
		return;

	objectCount = file->Read<UINT>();

	int count = 0;
	while (true)
	{
		std::string modelName = file->Read<std::string>();
		if (modelName.length() < 0)
			break;

		if(objectCount <= count)
			break;

		UINT objCount = file->Read<UINT>();
		for (UINT i = 0; i < objCount; i++)
		{
			std::string objectName = file->Read<std::string>();

			Vector3 pos = file->Read<Vector3>();
			Vector3 rot = file->Read<Vector3>();
			Vector3 scale = file->Read<Vector3>();

			// load model
			std::wstring wModelName = Utils::StringToWString(modelName);
			auto model = ResourceManager::GetInstance().Load<Model>(wModelName, wModelName + L"/" + wModelName, false);

			if (model == nullptr)
				continue;

			// make object
			std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
			obj->AddComponent(std::make_shared<ModelRenderer>(shader));

			obj->GetModelRenderer()->SetModel(model);
			int pass = model->HasAnimation() ? 1 : 0;
			obj->GetModelRenderer()->SetPass(pass);

			obj->GetTransform()->SetWorldPosition(pos);
			obj->GetTransform()->SetWorldRotation(rot);
			obj->GetTransform()->SetWorldScale(scale);

			obj->objectName  =  Utils::StringToWString(objectName);
			objectList.push_back(obj);
			SceneManager::GetInstance().GetCurrentScene()->Add(obj);

			++count;
		}
	}
}

void ObjectManager::ShowObjectPickingUI()
{
	if (pickObject != nullptr || selectedObjectIdx != -1)
	{
		if (pickObject == nullptr)
			pickObject = objectList[selectedObjectIdx];

		ImGui::Text("Object Position");

		bool iChange = 0;

		auto transform = pickObject->GetTransform();
		Vector3 pos = transform->GetLocalPosition();
		Vector3 rot = transform->GetLocalRotation();
		Vector3 scale = transform->GetLocalScale();

		ImGui::Text("Position");
		iChange += ImGui::InputFloat("Position X ##", &pos.x); 
		iChange += ImGui::InputFloat("Position Y ##", &pos.y); 
		iChange += ImGui::InputFloat("Position Z ##", &pos.z);

		ImGui::Text("Rotate");
		iChange += ImGui::InputFloat("Rotate X ##", &rot.x);
		iChange += ImGui::InputFloat("Rotate Y ##", &rot.y);
		iChange += ImGui::InputFloat("Rotate Z ##", &rot.z);
		ImGui::Text("Scale");
		iChange += ImGui::InputFloat("Scale X ##", &scale.x);
		iChange += ImGui::InputFloat("Scale Y ##", &scale.y);
		iChange += ImGui::InputFloat("Scale Z ##", &scale.z);

		//오브젝트 위치, 회전, 스케일 변경시 적용되는 조건문
		if (iChange > 0)
		{
			pickObject->GetTransform()->SetLocalPosition(pos);
			pickObject->GetTransform()->SetLocalRotation(rot);
			pickObject->GetTransform()->SetLocalScale(scale);
		}
		if (ImGui::Button("delete"))
		{
			objectList.erase(std::find(objectList.begin(), objectList.end(), pickObject));
			SceneManager::GetInstance().GetCurrentScene()->Remove(pickObject);

			pickObject = nullptr;

			if(objectList.size() > 0)	
				selectedObjectIdx = selectedObjectIdx % objectList.size();
			else 
				selectedObjectIdx = -1;

			--objectCount;
		}
	}
	else
	{
		ImGui::Text("[ Pick Object = nullptr ]");
	}
	
	ImGui::Text("SceneObject List");
	if (ImGui::BeginListBox("SceneObject List", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < objectList.size(); n++)
		{
			const bool is_selected = (selectedObjectIdx == n);
			if (ImGui::Selectable(Utils::WStringToString(objectList[n]->objectName).c_str(), is_selected))
			{
				pickObject = objectList[n];
				selectedObjectIdx = n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void ObjectManager::ShowBoundingBox(std::shared_ptr<DebugDrawer>& debugDraw)
{
	for (auto& obj : objectList)
		debugDraw->DrawBox(obj->GetTransform()->GetBoundingBox(), Color(0,0,1,0));
}

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{

}
