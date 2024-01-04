#include "pch.h"
#include "ObjectManager.h"

#include "SectionNode.h"

void ObjectManager::Init(std::wstring sceneFilePath)
{
	if(scene != nullptr)
		scene.reset();
	scene = std::make_shared<Scene>();

	if (sceneFilePath.size() > 1)
		scene->LoadScene(sceneFilePath);

	SceneManager::GetInstance().ChangeScene(scene);

	// shader
	shader = ResourceManager::GetInstance().Load<Shader>(L"ObjectShader", L"Shader/MapToolShader/ObjectShader.fx");
	RenderManager::GetInstance().Init(shader);

	ReadObjectModelNameList();
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

void ObjectManager::SpawnObject(const std::wstring& objectModelName, Vector3& spawnPoint, std::map<int, std::shared_ptr<SectionNode>>& leafNodeMap)
{
	if(selectedModelIdx < 0 || objectModelName.size() < 1)
		return;

	// load model
	std::wstring modelName = Utils::StringToWString(objectModelNameList[selectedModelIdx]);
	auto model = ResourceManager::GetInstance().Load<Model>(modelName, modelName + L"/" + modelName, false);

	if(model == nullptr)
		return;

	// make object
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	obj->AddComponent(std::make_shared<ModelRenderer>(shader));

	obj->GetModelRenderer()->SetModel(model);
	int pass = model->HasAnimation() ? 1 : 0;
	obj->GetModelRenderer()->SetPass(pass);

	obj->GetTransform()->SetWorldPosition(spawnPoint);
	objectList.push_back(obj);

	// set obj group node
	for (auto& leafNode : leafNodeMap)
	{
		if (Collision::CubeToCube(leafNode.second->boundingBox , obj->GetTransform()->GetBoundingBox()))
		{
			obj->groupNodeIdxList.push_back(leafNode.first);
			SceneManager::GetInstance().GetCurrentScene()->Add(obj, leafNode.first);
		}
	}
}

void ObjectManager::ShowUI()
{
	if (pickObject != nullptr)
	{
		bool iChange = 0;
		ImGui::PushItemWidth(50);
		//ImGui::Text( to_wm(pickObject->).c_str());
		//ImGui::Text(ImVec4(0.5f, 0.5f, 0.5f, 1), "Position"); ImGui::SameLine();
		//iChange += ImGui::InputFloat("X ##position", &m_MousePicker.m_pSeletedObj->obj_pos.x); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Y ##position", &m_MousePicker.m_pSeletedObj->obj_pos.y); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Z ##position", &m_MousePicker.m_pSeletedObj->obj_pos.z);
		//ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Rotation"); ImGui::SameLine();
		//iChange += ImGui::InputFloat("X ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.x); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Y ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.y); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Z ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.z);
		//ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Scale"); ImGui::SameLine();
		//iChange += ImGui::InputFloat("X ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.x); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Y ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.y); ImGui::SameLine();
		//iChange += ImGui::InputFloat("Z ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.z);
		////오브젝트 위치, 회전, 스케일 변경시 적용되는 조건문
		//if (iChange > 0)
		//{
		//	m_MousePicker.m_pSeletedObj->UpdateData();
		//	m_MousePicker.m_pSeletedObj->UpdateCollision();
		//	//오브젝트 이동시 새로운 노드에 넣는다.
		//	//기존 노드에 오브젝트 nullptr
		//	m_Terrian_Space.UpdateObject();
		//}
	}

	if (ImGui::BeginListBox("model list"))
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


	// open Dialog Simple
	if (ImGui::Button("Load FBX File"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".fbx", "../../Res/Assets/");

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{

}
