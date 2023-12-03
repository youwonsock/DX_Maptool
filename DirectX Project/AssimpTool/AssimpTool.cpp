#include "pch.h"
#include "AssimpTool.h"

#include "Converter.h"

void AssimpTool::Init()
{
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");
	}
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Idle.fbx");
		converter->ExportAnimationData(L"Kachujin/Idle");
	}
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Run.fbx");
		converter->ExportAnimationData(L"Kachujin/Run");
	}
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Slash.fbx");
		converter->ExportAnimationData(L"Kachujin/Slash");  
	}
}

void AssimpTool::FixedUpdate()
{
}

void AssimpTool::Update()
{
}

void AssimpTool::PostUpdate()
{
}

void AssimpTool::PreRender()
{
}

void AssimpTool::Render()
{
}

void AssimpTool::PostRender()
{
}

void AssimpTool::Release()
{
}
