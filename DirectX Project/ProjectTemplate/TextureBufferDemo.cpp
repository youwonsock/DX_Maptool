 #include "pch.h"
#include "TextureBufferDemo.h"

#include "Engine/TestCamera.h"

void TextureBufferDemo::Init()
{
	//compute Shader
	ComPtr<ID3D11ShaderResourceView> srv;
	{
		auto shader1 = std::make_shared<Shader>(L"26. TextureBufferDemo.fx");

		//auto texture = std::make_shared<Texture>();
		//texture->LoadDefaultFlagTexture(L"../../Res/Textures/Chim/Chim.jpg");
		auto texture = ResourceManager::GetInstance().Load<Texture>(L"Chim", L"../../Res/Textures/Chim/Chim.jpg");

		std::shared_ptr<TextureBuffer> textureBuffer = std::make_shared<TextureBuffer>();
		textureBuffer->CreateBuffer(texture->GetTexture2D());

		shader1->GetSRV("Input")->SetResource(textureBuffer->GetShaderResourceView().Get());
		shader1->GetUAV("Output")->SetUnorderedAccessView(textureBuffer->GetUnorderedAccessView().Get());

		UINT width = textureBuffer->GetWidth();
		UINT height = textureBuffer->GetHeight();
		UINT arraySize = textureBuffer->GetArraySize();

		UINT x = max((width + 31)/ 32, 1);
		UINT y = max((height + 31)/ 32, 1);
		shader1->Dispatch(0,0,x, y, arraySize);

		srv = textureBuffer->GetOutputSRV();
	}

	shader = std::make_shared<Shader>(L"23. RenderDemo.fx");

	// camera
	{
		std::shared_ptr<GameObject>cameraObject = std::make_shared<GameObject>();
		cameraObject->AddComponent(std::make_shared<TestCamera>());
		cameraObject->GetTransform()->SetWorldPosition(Vector3(0, 0, 0));
		cameraObject->GetTransform()->SetWorldRotation(Vector3(0, 0, 0));

		CameraManager::GetInstance().AddCamera(L"MainCamera", cameraObject);
	}


	// mesh

	// Material
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);

		auto texture = std::make_shared<Texture>();
		texture->SetShaderResourceView(srv);
		material->SetDiffuseMap(texture);

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vector4(1.f);
		desc.diffuse = Vector4(1.f);
		desc.specular = Vector4(1.f);

		ResourceManager::GetInstance().Add(L"Chim", material);
	}

	for (int i = 0; i < 500; ++i)
	{
		auto obj = std::make_shared<GameObject>();
		obj->GetTransform()->SetWorldPosition(Vector3(rand() % 100, 0, rand() % 100));
		obj->AddComponent(std::make_shared<MeshRenderer>());

		{
			obj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInstance().Get<Material>(L"Chim"));

			auto mesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
		}

		SceneManager::GetInstance().GetCurrentScene()->Add(obj);
	}


	RenderManager::GetInstance().Init(shader);
}
 
void TextureBufferDemo::FixedUpdate()
{
}

void TextureBufferDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}
}

void TextureBufferDemo::PostUpdate()
{
}

void TextureBufferDemo::PreRender()
{
}

void TextureBufferDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void TextureBufferDemo::PostRender()
{
}

void TextureBufferDemo::Release()
{
}
