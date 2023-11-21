#include "pch.h"
#include "BlendState.h"

void BlendState::CreateBlendState(D3D11_RENDER_TARGET_BLEND_DESC blendDesc, float factor)
{
	blendFactor = factor;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;
	blendStateDesc.RenderTarget[0] = blendDesc;

	HRESULT hr = Global::g_device->CreateBlendState(&blendStateDesc, blendState.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		ShowErrorMessage(hr);
}
