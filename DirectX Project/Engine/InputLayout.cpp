#include "pch.h"
#include "InputLayout.h"

void InputLayout::CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC> descVector, ComPtr<ID3DBlob> blob)
{
	UINT count = descVector.size();
	Global::g_device->CreateInputLayout(descVector.data(), count, blob->GetBufferPointer()
										, blob->GetBufferSize(), inputLayout.ReleaseAndGetAddressOf());
}
