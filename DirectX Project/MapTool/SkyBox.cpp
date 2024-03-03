#include "pch.h"
#include "SkyBox.h"

void SkyBox::Render()
{
	Matrix view  = CameraManager::GetInstance().GetMainCamera()->viewMatrix;
	Matrix proj = CameraManager::GetInstance().GetMainCamera()->projectionMatrix;

	shader->PushGlobalData(view, proj);

	TransformDesc transformDesc;
	transformDesc.World = Matrix::Identity;
	shader->PushTransformData(transformDesc);

	UINT stride = skyMesh->GetVertexBuffer()->GetStride();
	UINT offset = skyMesh->GetVertexBuffer()->GetOffset();

	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Global::g_immediateContext->IASetVertexBuffers(0, 1, skyMesh->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(skyMesh->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, skyMesh->GetIndexBuffer()->GetIndexCount());

}

SkyBox::SkyBox()
{
	shader = ResourceManager::GetInstance().Load<Shader>(L"SkyboxShader", L"Shader/MapToolShader/SkyBox.fx");
	texture = std::make_shared<Texture>();
	texture->LoadCubemapTexture(L"../../Res/Textures/Terrain/grassenvmap1024.dds");

	skyMesh = ResourceManager::GetInstance().Get<Mesh>(L"Cube");

	ComPtr<ID3DX11EffectShaderResourceVariable> cubemapSRV = shader->GetSRV("CubeMapTexture");
	HRESULT hr = cubemapSRV->SetResource(texture->GetShaderResourceView().Get());
}

SkyBox::~SkyBox()
{
}
