#pragma once
class RasterizerState
{
private:
	ComPtr<ID3D11RasterizerState> rasterizerState;

public:
	ComPtr<ID3D11RasterizerState> GetRasterizerState() { return rasterizerState; }

	void CreateRasterizerState();
};

