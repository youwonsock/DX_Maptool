#pragma once
class BlendState
{
private:
	ComPtr<ID3D11BlendState> blendState;
	float blendFactor = 0.0f;
	UINT sampleMask = 0xffffffff;

public:
	ComPtr<ID3D11BlendState> GetBlendState() { return blendState; }
	const float& GetBlendFactor() { return blendFactor; }
	const UINT& GetSampleMask() { return sampleMask; }

	void CreateBlendState(D3D11_RENDER_TARGET_BLEND_DESC blendDesc =
							{
								true,
								D3D11_BLEND_SRC_ALPHA,
								D3D11_BLEND_INV_SRC_ALPHA,
								D3D11_BLEND_OP_ADD,
								D3D11_BLEND_ONE,
								D3D11_BLEND_ZERO,
								D3D11_BLEND_OP_ADD,
								D3D11_COLOR_WRITE_ENABLE_ALL
							}
							, float factor = 0);
};

