#include "pch.h"
#include "AssimpTool.h"

#include "Converter.h"

void AssimpTool::Init()
{
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"Tank/Tank.fbx");

		// Memory -> CustomData (File)
		converter->ExportMaterialData(L"Tank/Tank");
		converter->ExportModelData(L"Tank/Tank");

		// CustomData (File) -> Memory
	}

	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"Tower/Tower.fbx");

		// Memory -> CustomData (File)
		converter->ExportMaterialData(L"Tower/Tower");
		converter->ExportModelData(L"Tower/Tower");

		// CustomData (File) -> Memory
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
