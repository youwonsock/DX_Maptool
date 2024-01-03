#pragma once
class MapRenderer
{
private:
	std::shared_ptr<Shader> shader;

	std::shared_ptr<ConstantBuffer<GlobalDesc>> globalBuffer;
	std::shared_ptr<ConstantBuffer<TransformDesc>> transformBuffer;

	ComPtr<ID3DX11EffectConstantBuffer> globalEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> transformEffectBuffer;

	GlobalDesc globalDesc;
	TransformDesc transformDesc;

public:
	void Update();

	MapRenderer();
	~MapRenderer();
};

