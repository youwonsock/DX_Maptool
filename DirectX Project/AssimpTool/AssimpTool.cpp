#include "pch.h"
#include "AssimpTool.h"

#include "Converter.h"

void AssimpTool::Init()
{
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		// fbx -> memory
		converter->ReadAssetFile(L"House/House.fbx");

		// memory -> customData
		converter->ExportMaterialData(L"House/House");
		converter->ExportModelData(L"House/House");

		// customData -> memory or file
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
