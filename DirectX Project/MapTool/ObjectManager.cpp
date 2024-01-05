#include "pch.h"
#include "ObjectManager.h"

#include "SectionNode.h"
#include "AssimpTool\Converter.h"

void ObjectManager::Init(std::wstring sceneFilePath)
{
	SceneManager::GetInstance().GetCurrentScene()->LoadScene(sceneFilePath);
	// shader
	shader = ResourceManager::GetInstance().Load<Shader>(L"ObjectShader", L"Shader/MapToolShader/ObjectShader.fx");
	RenderManager::GetInstance().Init(shader);

	ReadObjectModelNameList();
	objectCount = 0;
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

std::shared_ptr<GameObject> ObjectManager::SpawnObject(Vector3& spawnPoint)
{
	std::shared_ptr<GameObject> obj = nullptr;

	if(selectedModelIdx < 0)
		return obj;

	// load model
	std::wstring modelName = Utils::StringToWString(objectModelNameList[selectedModelIdx]);
	auto model = ResourceManager::GetInstance().Load<Model>(modelName, modelName + L"/" + modelName, false);

	if(model == nullptr)
		return obj;

	// make object
	obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<ModelRenderer>(shader));

	obj->GetModelRenderer()->SetModel(model);
	int pass = model->HasAnimation() ? 1 : 0;
	obj->GetModelRenderer()->SetPass(pass);

	obj->GetTransform()->SetWorldPosition(spawnPoint);
	objectList.push_back(obj);

	obj->objectName = modelName + std::to_wstring(objectCount++);

	return obj;
}

void ObjectManager::ObjectPicking(Ray& ray)
{
	for (auto& obj : objectList)
	{
		if (Collision::CubeToRay(obj->GetTransform()->GetBoundingBox(), ray))
		{
			pickObject = obj;
			selectedObjectIdx = -1;
			return;
		}
	}

	pickObject = nullptr; 
	selectedObjectIdx = -1;
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
	ImGui::Checkbox("Use FBXSDK", &useFBXSDK);


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

			UseLib type = UseLib::ASSIMP;
			if (useFBXSDK)
				type = UseLib::FBXSDK;
			converter->ExportAnimationData(wFileName + L"/" + wFileName, 0, type);

			ReadObjectModelNameList();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

std::shared_ptr<GameObject> ObjectManager::ShowObjectPickingUI()
{
	if (pickObject != nullptr || selectedObjectIdx != -1)
	{
		if (pickObject == nullptr)
			pickObject = objectList[selectedObjectIdx];

		ImGui::Text("Object Position");

		bool iChange = 0;

		auto transform = pickObject->GetTransform();
		Vector3 pos = transform->GetWorldPosition();
		Vector3 rot = transform->GetWorldRotation();
		Vector3 scale = transform->GetWorldScale();

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
			pickObject->GetTransform()->SetWorldPosition(pos);
			pickObject->GetTransform()->SetWorldRotation(rot);
			pickObject->GetTransform()->SetWorldScale(scale);
		}
		if (ImGui::Button("delete"))
		{
			objectList.erase(std::find(objectList.begin(), objectList.end(), pickObject));

			for (auto& nodeIdx : pickObject->groupNodeIdxList)
				SceneManager::GetInstance().GetCurrentScene()->Remove(pickObject, nodeIdx);

			pickObject = nullptr;
			selectedObjectIdx = -1;
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

	return pickObject;
}

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{

}
