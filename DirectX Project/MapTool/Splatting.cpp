#include "pch.h"
#include "Splatting.h"

void Splatting::TillingTexture(Vector3 centerPos, float brushSize, std::vector<PNCTVertex>& vertexList, std::vector<UINT>& updateIdxList)
{
	float distance = 0.0f;
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	Vector2 center = Vector2(centerPos.x, centerPos.z);
	for (UINT i : updateIdxList)
	{
		distance = (Vector2(vertexList[i].position.x, vertexList[i].position.z) - center).Length();

		distance = (Vector2(vertexList[i].position.x, vertexList[i].position.z) - center).Length();
		distance = (distance / brushSize);
		distance = -(distance - 1);
		distance *= deltaTime * brushScale;

		switch (tillingTexNum)
		{
		case(0):
			vertexList[i].color.x += distance;
			vertexList[i].color.x = std::clamp(vertexList[i].color.x, 0.0f, 255.0f);
			break;
		case(1):
			vertexList[i].color.y += distance;
			vertexList[i].color.y = std::clamp(vertexList[i].color.y, 0.0f, 255.0f);
			break;
		case(2):
			vertexList[i].color.z += distance;
			vertexList[i].color.z = std::clamp(vertexList[i].color.z, 0.0f, 255.0f);
			break;
		case(3):
			vertexList[i].color.w += distance;
			vertexList[i].color.w = std::clamp(vertexList[i].color.w, 0.0f, 255.0f);
			break;
		case(4):
			vertexList[i].color.x = 0;
			vertexList[i].color.y = 0;
			vertexList[i].color.z = 0;
			vertexList[i].color.w = 0;
			break;
		default:
			break;
		}
	}

	//temp : update alpha texture
	std::vector<BYTE> colorList;
	colorList.resize(vertexList.size() * 4);

	for (int i = 0; i < vertexList.size(); ++i)
	{
		colorList[i * 4 + 0] = vertexList[i].color.x;
		colorList[i * 4 + 1] = vertexList[i].color.y;
		colorList[i * 4 + 2] = vertexList[i].color.z;
		colorList[i * 4 + 3] = vertexList[i].color.w;
	}

	alphaTexture->UpdateTexture(colorList);
}

void Splatting::SetVertexByTexture(std::vector<PNCTVertex>& vertexList)
{
	std::vector<BYTE> colorList;

	alphaTexture->GetTextureRGBAData(colorList);

	for (int i = 0; i < vertexList.size(); ++i)
	{
		vertexList[i].color.x = colorList[i * 4 + 0];
		vertexList[i].color.y = colorList[i * 4 + 1];
		vertexList[i].color.z = colorList[i * 4 + 2];
		vertexList[i].color.w = colorList[i * 4 + 3];
	}
}

void Splatting::SaveAlphaTexture(std::wstring savePath)
{
	alphaTexture->SaveTexture(savePath);
}

void Splatting::ShowUI()
{
	ImGui::SliderFloat("Splatting Scale", &brushScale, 500, 5000);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Tilling Texture Num");
	ImGui::RadioButton("1", &tillingTexNum, 0); ImGui::SameLine();
	ImGui::RadioButton("2", &tillingTexNum, 1); ImGui::SameLine();
	ImGui::RadioButton("3", &tillingTexNum, 2); ImGui::SameLine();
	ImGui::RadioButton("4", &tillingTexNum, 3); ImGui::SameLine();
	ImGui::RadioButton("Erase", &tillingTexNum, 4);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Texture1"); ImGui::SameLine(90);
	ImGui::Text("Texture2"); ImGui::SameLine(180);
	ImGui::Text("Texture3"); ImGui::SameLine(270);
	ImGui::Text("Texture4");
	ImGui::Image(texture1->GetShaderResourceView().Get(), ImVec2(50, 50)); ImGui::SameLine(90);
	ImGui::Image(texture2->GetShaderResourceView().Get(), ImVec2(50, 50)); ImGui::SameLine(180);
	ImGui::Image(texture3->GetShaderResourceView().Get(), ImVec2(50, 50)); ImGui::SameLine(270);
	ImGui::Image(texture4->GetShaderResourceView().Get(), ImVec2(50, 50));

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGuiFileDialog::Instance()->OpenDialog("embedded", "Select File"
			, "Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg}"
			, "../../Res/Textures/",1, nullptr,
			ImGuiFileDialogFlags_NoDialog |
			ImGuiFileDialogFlags_DisableBookmarkMode |
			ImGuiFileDialogFlags_DisableCreateDirectoryButton |
			ImGuiFileDialogFlags_DisableQuickPathSelection |
			ImGuiFileDialogFlags_DisableThumbnailMode |
			ImGuiFileDialogFlags_HideColumnDate |
			ImGuiFileDialogFlags_HideColumnSize |
			ImGuiFileDialogFlags_HideColumnType);

	if (ImGuiFileDialog::Instance()->Display("embedded", ImGuiWindowFlags_NoCollapse, ImVec2(0, 0), ImVec2(300, 300)))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			switch (tillingTexNum)
			{
			case(0):
				texture1->Load(Utils::StringToWString(filePathName));
				shader->GetSRV("Texture1")->SetResource(texture1->GetShaderResourceView().Get());
				break;
			case(1):
				texture2->Load(Utils::StringToWString(filePathName));
				shader->GetSRV("Texture2")->SetResource(texture2->GetShaderResourceView().Get());
				break;
			case(2):
				texture3->Load(Utils::StringToWString(filePathName));
				shader->GetSRV("Texture3")->SetResource(texture3->GetShaderResourceView().Get());
				break;
			case(3):
				texture4->Load(Utils::StringToWString(filePathName));
				shader->GetSRV("Texture4")->SetResource(texture4->GetShaderResourceView().Get());
				break;
			default:
				texture1->Load(Utils::StringToWString(filePathName));
				shader->GetSRV("Texture1")->SetResource(texture1->GetShaderResourceView().Get());
				break;
			}
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void Splatting::Init(SplattingDesc& desc)
{
	// texture is BGRA so in imgui Red is 2, Green is 1, Blue is 0, Alpha is 3
	shader = ResourceManager::GetInstance().Get<Shader>(L"MapToolShader");

	texture1 = std::make_shared<Texture>();
	texture1->Load(desc.texture1Path);

	texture2 = std::make_shared<Texture>();
	texture2->Load(desc.texture2Path);

	texture3 = std::make_shared<Texture>();
	texture3->Load(desc.texture3Path);

	texture4 = std::make_shared<Texture>();
	texture4->Load(desc.texture4Path);

	if (alphaTexture == nullptr)
		alphaTexture = std::make_shared<Texture>();

	if (!alphaTexture->Load(desc.alphaTexPath))
		alphaTexture->CreateTexture(desc.rowNum, desc.colNum);

	SetSRV();
}

/// <summary>
/// set srv
/// </summary>
void Splatting::SetSRV()
{
	shader->GetSRV("MapAlphaTexture")->SetResource(alphaTexture->GetShaderResourceView().Get());

	shader->GetSRV("Texture1")->SetResource(texture1->GetShaderResourceView().Get());
	shader->GetSRV("Texture2")->SetResource(texture2->GetShaderResourceView().Get());
	shader->GetSRV("Texture3")->SetResource(texture3->GetShaderResourceView().Get());
	shader->GetSRV("Texture4")->SetResource(texture4->GetShaderResourceView().Get());
}
