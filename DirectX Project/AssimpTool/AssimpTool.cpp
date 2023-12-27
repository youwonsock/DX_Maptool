#include "pch.h"
#include "AssimpTool.h"

#include "Converter.h"

void AssimpTool::Init()
{
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
	//	converter->ExportMaterialData(L"Kachujin/Kachujin");
	//	converter->ExportModelData(L"Kachujin/Kachujin");
	//}
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"Kachujin/Idle.fbx");
	//	converter->ExportAnimationData(L"Kachujin/Idle");
	//}
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"Kachujin/Run.fbx");
	//	converter->ExportAnimationData(L"Kachujin/Run");
	//}
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"Kachujin/Slash.fbx");
	//	converter->ExportAnimationData(L"Kachujin/Slash");
	//}

	/*{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Turret_Deploy1/Turret_Deploy1.fbx");
		converter->ExportMaterialData(L"Turret_Deploy1/Turret_Deploy1");
		converter->ExportModelData(L"Turret_Deploy1/Turret_Deploy1");
		converter->ExportAnimationData(L"Turret_Deploy1/Turret_Deploy1");
	}

	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"ship/ship.fbx");
		converter->ExportMaterialData(L"ship/ship");
		converter->ExportModelData(L"ship/ship");
	}*/

	// tower
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"Tower/Tower.fbx");
	//	converter->ExportMaterialData(L"Tower/Tower");
	//	converter->ExportModelData(L"Tower/Tower");
	//}

	//// multi camera
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"MultiCameras/MultiCameras.fbx");
	//	converter->ExportMaterialData(L"MultiCameras/MultiCameras");
	//	converter->ExportModelData(L"MultiCameras/MultiCameras");
	//}

	// ship
	//{
	//	std::shared_ptr<Converter> converter = std::make_shared<Converter>();

	//	converter->ReadAssetFile(L"ship/ship.fbx");
	//	converter->ExportMaterialData(L"ship/ship");
	//	converter->ExportModelData(L"ship/ship");
	//}


	// turret
	{
		std::shared_ptr<Converter> converter = std::make_shared<Converter>();

		converter->ReadAssetFile(L"Turret_Deploy1/Turret_Deploy1.fbx");
		converter->ExportMaterialData(L"Turret_Deploy1/Turret_Deploy1");
		converter->ExportModelData(L"Turret_Deploy1/Turret_Deploy1");
		converter->ExportAnimationData(L"Turret_Deploy1/Turret_Deploy1");
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
