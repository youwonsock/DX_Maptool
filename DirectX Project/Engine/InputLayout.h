#pragma once
class InputLayout
{
private:
	ComPtr<ID3D11InputLayout> inputLayout;

public:
	ComPtr<ID3D11InputLayout> GetInputLayout() { return inputLayout; }

	void CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC> descVector, ComPtr<ID3DBlob> blob);
};

