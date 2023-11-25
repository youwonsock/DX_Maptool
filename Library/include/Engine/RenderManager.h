#pragma once

#include "Singleton.hpp"
#include "Struct.h"

class Shader;

template <typename T>
class ConstantBuffer;

struct GlobalDesc 
{
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	Matrix VirwProjection = Matrix::Identity;
};

struct TransformDesc
{
	Matrix World = Matrix::Identity;
};

class RenderManager : public Singleton<RenderManager>
{
private:
	friend class Singleton<RenderManager>;
	RenderManager() {};

	std::shared_ptr<Shader> shader;
	std::shared_ptr<ConstantBuffer<GlobalDesc>> globalBuffer;
	std::shared_ptr<ConstantBuffer<TransformDesc>> transformBuffer;

	ComPtr<ID3DX11EffectConstantBuffer> globalEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> transformEffectBuffer;

	TransformDesc transformDesc;
	GlobalDesc globalDesc;

public:
	void Init(const std::shared_ptr<Shader>& shader);
	void Update();

	void SetShader(const std::shared_ptr<Shader>& shader);
	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);
};

