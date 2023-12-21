#include "pch.h"
#include "SkyBox.h"

void SkyBox::Render()
{
	shader->GetSRV("Texture0")->SetResource(texture->GetShaderResourceView().Get());
	shader->GetMatrix("view")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->viewMatrix);
	shader->GetMatrix("projection")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->projectionMatrix);

	UINT stride = skyMesh->GetVertexBuffer()->GetStride();
	UINT offset = skyMesh->GetIndexBuffer()->GetOffset();

	Global::g_immediateContext->IASetVertexBuffers(0, 1, skyMesh->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(skyMesh->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, skyMesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

SkyBox::SkyBox()
{
	shader = std::make_unique<Shader>(L"MapToolShader/SkyBox.fx");
	texture = ResourceManager::GetInstance().GetTexture(L"sky", L"../../Res/Textures/Sky01.jpg");

	skyMesh = ResourceManager::GetInstance().Get<Mesh>(L"Sphere");
}

SkyBox::~SkyBox()
{
}
