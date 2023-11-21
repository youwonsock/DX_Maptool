#include "pch.h"
#include "RasterizerState.h"

void RasterizerState::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;

	HRESULT hr = Global::g_device->CreateRasterizerState(&rasterizerDesc, rasterizerState.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		ShowErrorMessage(hr);
}
