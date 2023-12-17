 #include "pch.h"
#include "InstancingDemo.h"

#include "Engine/TestCamera.h"

void InstancingDemo::Init()
{
	// camera
	{	
		cameraObject = std::make_shared<GameObject>();
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, -10));
		cameraObject->AddComponent(std::make_shared<Camera>());
		cameraObject->AddComponent(std::make_shared<TestCamera>());
	}
	
	shader = std::make_shared<Shader>(L"19. InstancingDemo.fx");

	// Material
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		auto texture = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"../../Res/Textures/Chim/YellowLemonChim.png");
		material->SetDiffuseMap(texture);

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vector4(1.f);
		desc.diffuse = Vector4(1.f);
		desc.specular = Vector4(1.f);

		ResourceManager::GetInstance().Add(L"Chim", material);

		m_material = material;
	}

	for (int i = 0; i < 100000; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 10, 0, rand() % 10));
		obj->AddComponent(std::make_shared<MeshRenderer>());

		{
			obj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInstance().Get<Material>(L"Chim"));

			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);

			m_mesh = mesh;
		}
		
		objs.push_back(obj);
	}

	RenderManager::GetInstance().Init(shader);

	// instance buffer 
	{
		instanceBuffer = std::make_shared<VertexBuffer>();

		// set world matrixs
		for (auto obj : objs)
		{
			worldMats.push_back(obj->GetTransform()->GetWorldMatrix());
		}

		instanceBuffer->CreateVertexBuffer(worldMats, 1);
	}
}
 
void InstancingDemo::FixedUpdate()
{
}

void InstancingDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}

	cameraObject->Update();
	RenderManager::GetInstance().Update();
}

void InstancingDemo::PostUpdate()
{
}

void InstancingDemo::PreRender()
{
}

Vector3 rotate1 = Vector3(0,0,0);
Vector3 rotate2 = Vector3(0,0,0);
void InstancingDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set global light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vector4(1.f);
		lightDesc.diffuse = Vector4(1.f);
		lightDesc.specular = Vector4(1.f);
		lightDesc.emissive = Vector4(1.0f);
		lightDesc.direction = Vector3(1.0f, 0.f, 1.0f);

		RenderManager::GetInstance().PushLightData(lightDesc);
	}

	//for (auto obj : objs)
	//	obj->Render();

	m_material->Update();

	m_mesh->GetVertexBuffer()->PushData();
	instanceBuffer->PushData();
	m_mesh->GetIndexBuffer()->PushData();

	shader->DrawIndexedInstanced(0,0, m_mesh->GetIndexBuffer()->GetIndexCount(),objs.size());
}

void InstancingDemo::PostRender()
{
}

void InstancingDemo::Release()
{
}
