#include "pch.h"
#include "ResourceManager.h"

#include "ResourceHeader.h"
#include "PipeLineHeaders.h"

void ResourceManager::CreateDefaultTexture()
{
	{
		auto texture = std::make_shared<Texture>();
		texture->SetName(L"Chim");
		texture->CreateTexture(L"../../Res/IMG/Chim.jpg");
		Add(texture->GetName(), texture);
	}
	{
		auto texture = std::make_shared<Texture>();
		texture->SetName(L"Snake");
		texture->CreateTexture(L"../../Res/IMG/Snake.bmp");
		Add(texture->GetName(), texture);
	}
}

void ResourceManager::CreateDefaultMaterial()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->SetName(L"Default");
	material->SetShader(Get<ShaderRes>(L"Default"));
	material->SetTexture(Get<Texture>(L"Chim"));
	Add(material->GetName(), material);
}

void ResourceManager::CreateDefaultShader()
{
	auto vertexShader = std::make_shared<VertexShader>();
	vertexShader->Create(L"../../Res/Shader/Default.hlsl", "VS", "vs_5_0");
	auto inputLayout = std::make_shared<InputLayout>();
	inputLayout->CreateInputLayout(VertexTextureData::descs, vertexShader->GetBlob());
	auto pixelShader = std::make_shared<PixelShader>();
	pixelShader->Create(L"../../Res/Shader/Default.hlsl", "PS", "ps_5_0");

	// Shader
	std::shared_ptr<ShaderRes> shader = std::make_shared<ShaderRes>();
	shader->SetName(L"Default");
	shader->SetVertexShader(vertexShader);
	shader->SetInputLayout(inputLayout);
	shader->SetPixelShader(pixelShader);
	Add(shader->GetName(), shader);
}

void ResourceManager::CreateDefaultMesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->SetName(L"Rectangle");
	mesh->CreateDefaultRactangle();
	Add(mesh->GetName(), mesh);
}

void ResourceManager::CreateDefaultAnimation()
{
	std::shared_ptr<Animation> animation = std::make_shared<Animation>();
	animation->SetName(L"SnakeAnim");
	animation->SetTexture(Get<Texture>(L"Snake"));
	animation->SetIsLooping(true);

	animation->AddKeyFrame(KeyFrame{ Vector2{0.f, 0.f}, Vector2{100.f, 100.f}, 0.1f });
	animation->AddKeyFrame(KeyFrame{ Vector2{100.f, 0.f}, Vector2{100.f, 100.f}, 0.1f });
	animation->AddKeyFrame(KeyFrame{ Vector2{200.f, 0.f}, Vector2{100.f, 100.f}, 0.1f });
	animation->AddKeyFrame(KeyFrame{ Vector2{300.f, 0.f}, Vector2{100.f, 100.f}, 0.1f });

	Add(animation->GetName(), animation);

	// XML + JSON
	animation->Save(L"TestAnim.xml");

	//for test load function
	std::shared_ptr<Animation> anim2 = std::make_shared<Animation>();
	anim2->Load(L"TestAnim.xml");
}

void ResourceManager::Init()
{
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultAnimation();
}
