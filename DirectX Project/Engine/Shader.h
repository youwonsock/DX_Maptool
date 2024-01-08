#pragma once
#include "Pass.h"
#include "Technique.h"
#include "Struct.h"

class Shader : public std::enable_shared_from_this<Shader>, public ResourceBase
{
public:
	friend struct Pass;

public:
	Shader();
	Shader(std::wstring file);
	~Shader();

	std::wstring GetFile();
	ComPtr<ID3DX11Effect> Effect();

	void Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);

	void Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z);

	ComPtr<ID3DX11EffectVariable> GetVariable(std::string name);
	ComPtr<ID3DX11EffectScalarVariable> GetScalar(std::string name);
	ComPtr<ID3DX11EffectVectorVariable> GetVector(std::string name);
	ComPtr<ID3DX11EffectMatrixVariable> GetMatrix(std::string name);
	ComPtr<ID3DX11EffectStringVariable> GetString(std::string name);
	ComPtr<ID3DX11EffectShaderResourceVariable> GetSRV(std::string name);
	ComPtr<ID3DX11EffectRenderTargetViewVariable> GetRTV(std::string name);
	ComPtr<ID3DX11EffectDepthStencilViewVariable> GetDSV(std::string name);
	ComPtr<ID3DX11EffectUnorderedAccessViewVariable> GetUAV(std::string name);
	ComPtr<ID3DX11EffectConstantBuffer> GetConstantBuffer(std::string name);
	ComPtr<ID3DX11EffectShaderVariable> GetShader(std::string name);
	ComPtr<ID3DX11EffectBlendVariable> GetBlend(std::string name);
	ComPtr<ID3DX11EffectDepthStencilVariable> GetDepthStencil(std::string name);
	ComPtr<ID3DX11EffectRasterizerVariable> GetRasterizer(std::string name);
	ComPtr<ID3DX11EffectSamplerVariable> GetSampler(std::string name);

	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);
	void PushBoneData(const BoneDesc& desc);
	void PushKeyframeData(const KeyframeDesc& desc);
	void PushInstancedKeyFrameData(const InstancedKeyFrameDesc& desc);
	void PushTweenData(const TweenDesc& desc);
	void PushInstancedTweenData(const InstancedTweenDesc& desc);

	// resource base override
	virtual bool Load(const std::wstring& path) override;
private:
	void CreateEffect();
	ComPtr<ID3D11InputLayout> CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& params);

private:
	std::wstring file;
	ShaderDesc shaderDesc;
	D3DX11_EFFECT_DESC effectDesc;
	std::shared_ptr<StateBlock> initialStateBlock;
	std::vector<Technique> techniques;

	std::shared_ptr<ConstantBuffer<GlobalDesc>> globalBuffer;
	std::shared_ptr<ConstantBuffer<TransformDesc>> transformBuffer;
	std::shared_ptr<ConstantBuffer<LightDesc>> lightBuffer;
	std::shared_ptr<ConstantBuffer<MaterialDesc>> materialBuffer;
	std::shared_ptr<ConstantBuffer<BoneDesc>> boneBuffer;
	std::shared_ptr<ConstantBuffer<KeyframeDesc>> keyframeBuffer;
	std::shared_ptr<ConstantBuffer<TweenDesc>> tweenBuffer;
	std::shared_ptr<ConstantBuffer<InstancedTweenDesc>> instancedTweenBuffer;
	std::shared_ptr<ConstantBuffer<InstancedKeyFrameDesc>> instancedKeyFrameBuffer;

	ComPtr<ID3DX11EffectConstantBuffer> globalEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> transformEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> lightEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> materialEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> boneEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> keyframeEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> tweenEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> instancedTweenEffectBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> instancedKeyFrameEffectBuffer;

	TransformDesc transformDesc;
	GlobalDesc globalDesc;
	LightDesc lightDesc;
	MaterialDesc materialDesc;
	BoneDesc boneDesc;
	KeyframeDesc keyframeDesc;
	TweenDesc tweenDesc;
	InstancedTweenDesc instancedTweenDesc;
	InstancedKeyFrameDesc instancedKeyFrameDesc;
};
