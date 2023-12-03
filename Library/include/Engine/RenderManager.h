#pragma once

#include "Singleton.hpp"
#include "Struct.h"

#define MAX_MODEL_TRANSFORMS 250

class Shader;

template <typename T>
class ConstantBuffer;

struct GlobalDesc 
{
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	Matrix VirwProjection = Matrix::Identity;
	Matrix ViewInverse = Matrix::Identity;
};

struct TransformDesc
{
	Matrix World = Matrix::Identity;
};

struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(1.f, 1.f, 1.f, 1.f);
	
	Vector3 direction = Vector3(0.f, 0.f, 0.f);
	float pad0;
};

struct MaterialDesc
{
	Color ambient = Color(0.f, 0.f, 0.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(0.f, 0.f, 0.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
};

struct BoneDesc
{
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

class RenderManager : public Singleton<RenderManager>
{
private:
	friend class Singleton<RenderManager>;
	RenderManager() {};

	std::shared_ptr<Shader> shader;

	std::shared_ptr<ConstantBuffer<GlobalDesc>> globalBuffer;
	std::shared_ptr<ConstantBuffer<TransformDesc>> transformBuffer;
	std::shared_ptr<ConstantBuffer<LightDesc>> lightBuffer;
	std::shared_ptr<ConstantBuffer<MaterialDesc>> materialBuffer;
	std::shared_ptr<ConstantBuffer<BoneDesc>> boneBuffer;

	ComPtr<ID3DX11EffectConstantBuffer> globalEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> transformEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> lightEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> materialEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> boneEffectBuffer;

	TransformDesc transformDesc;
	GlobalDesc globalDesc;
	LightDesc lightDesc;
	MaterialDesc materialDesc;
	BoneDesc boneDesc;
	
public:
	void Init(const std::shared_ptr<Shader>& shader);
	void Update();

	void SetShader(const std::shared_ptr<Shader>& shader);

	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);
	void PushBoneData(const BoneDesc& desc);
};

