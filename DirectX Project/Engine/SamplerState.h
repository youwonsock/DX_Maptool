#pragma once
class SamplerState
{
private:
	ComPtr<ID3D11SamplerState> samplerState;

public:
	ComPtr<ID3D11SamplerState> GetSamplerState() { return samplerState; }

	void CreateSamplerState();
};

