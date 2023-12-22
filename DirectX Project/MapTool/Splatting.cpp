#include "pch.h"
#include "Splatting.h"

void Splatting::TillingTexture(Vector3 centerPos, int tillingTexNum, std::vector<PNCTVertex>& vertexList, std::vector<UINT>& updateIdxList)
{
	float distance = 0.0f;
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	for (UINT i : updateIdxList)
	{
		distance = (vertexList[i].position - centerPos).Length();

		if (distance < 1.0f)
			distance = 1.0;

		distance = (1 / distance);
		distance *= 100.0f;

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

void Splatting::Init(SplattingDesc& desc)
{
	// texture is BGRA so in imgui Red is 2, Green is 1, Blue is 0, Alpha is 3

	texture1 = std::make_shared<Texture>();
	texture1->Load(desc.texture1Path);

	texture2 = std::make_shared<Texture>();
	texture2->Load(desc.texture2Path);

	texture3 = std::make_shared<Texture>();
	texture3->Load(desc.texture3Path);

	texture4 = std::make_shared<Texture>();
	texture4->Load(desc.texture4Path);

	alphaTexture = std::make_shared<Texture>();

	if (!alphaTexture->Load(desc.alphaTexPath))
		alphaTexture->CreateTexture(desc.rowNum, desc.colNum);
}

/// <summary>
/// set srv
/// </summary>
void Splatting::SetSRV(std::shared_ptr<Shader> shader)
{
	shader->GetSRV("MapAlphaTexture")->SetResource(alphaTexture->GetShaderResourceView().Get());

	shader->GetSRV("Texture1")->SetResource(texture1->GetShaderResourceView().Get());
	shader->GetSRV("Texture2")->SetResource(texture2->GetShaderResourceView().Get());
	shader->GetSRV("Texture3")->SetResource(texture3->GetShaderResourceView().Get());
	shader->GetSRV("Texture4")->SetResource(texture4->GetShaderResourceView().Get());
}
