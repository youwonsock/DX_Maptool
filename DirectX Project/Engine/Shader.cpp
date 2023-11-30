#include "pch.h"
#include "Shader.h"

#include "ShaderManager.h"

Shader::Shader(std::wstring file) : file(L"..\\..\\Shader\\" + file)
{
	initialStateBlock = std::make_shared<StateBlock>();
	{
		Global::g_immediateContext->RSGetState(initialStateBlock->RSRasterizerState.GetAddressOf());
		Global::g_immediateContext->OMGetBlendState(initialStateBlock->OMBlendState.GetAddressOf(), initialStateBlock->OMBlendFactor, &initialStateBlock->OMSampleMask);
		Global::g_immediateContext->OMGetDepthStencilState(initialStateBlock->OMDepthStencilState.GetAddressOf(), &initialStateBlock->OMStencilRef);
	}

	CreateEffect();
}

Shader::~Shader()
{
	
}

std::wstring Shader::GetFile()
{
	return file; 
}

ComPtr<ID3DX11Effect> Shader::Effect()
{
	return shaderDesc.effect; 
}

void Shader::CreateEffect()
{
	shaderDesc = ShaderManager::GetInstance().GetEffect(file);

	shaderDesc.effect->GetDesc(&effectDesc);
	for (UINT t = 0; t < effectDesc.Techniques; t++)
	{
		Technique technique;
		technique.technique = shaderDesc.effect->GetTechniqueByIndex(t);
		technique.technique->GetDesc(&technique.desc);
		technique.name = StringToWString(technique.desc.Name);

		for (UINT p = 0; p < technique.desc.Passes; p++)
		{
			Pass pass;
			pass.pass = technique.technique->GetPassByIndex(p);
			pass.pass->GetDesc(&pass.desc);
			pass.name = StringToWString(pass.desc.Name);
			pass.pass->GetVertexShaderDesc(&pass.passVsDesc);
			pass.passVsDesc.pShaderVariable->GetShaderDesc(pass.passVsDesc.ShaderIndex, &pass.effectVsDesc);

			for (UINT s = 0; s < pass.effectVsDesc.NumInputSignatureEntries; s++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC desc;

				HRESULT hr = pass.passVsDesc.pShaderVariable->GetInputSignatureElementDesc(pass.passVsDesc.ShaderIndex, s, &desc);
				
				if (FAILED(hr))
					ShowErrorMessage(hr);

				pass.signatureDescs.push_back(desc);
			}

			pass.inputLayout = CreateInputLayout(shaderDesc.blob, &pass.effectVsDesc, pass.signatureDescs);
			pass.stateBlock = initialStateBlock;

			technique.passes.push_back(pass);
		}

		techniques.push_back(technique);
	}

	for (UINT i = 0; i < effectDesc.ConstantBuffers; i++)
	{
		ID3DX11EffectConstantBuffer* iBuffer;
		iBuffer = shaderDesc.effect->GetConstantBufferByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iBuffer->GetDesc(&vDesc);
	}

	for (UINT i = 0; i < effectDesc.GlobalVariables; i++)
	{
		ID3DX11EffectVariable* effectVariable;
		effectVariable = shaderDesc.effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		effectVariable->GetDesc(&vDesc);
	}
}

ComPtr<ID3D11InputLayout> Shader::CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& params)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (D3D11_SIGNATURE_PARAMETER_DESC& paramDesc : params)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		std::string name = paramDesc.SemanticName;
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);

		if (name == "POSITION")
		{
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}

		if (StartsWith(name, "INST") == true)
		{
			elementDesc.InputSlot = 1;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		if (StartsWith(name, "SV_") == false)
			inputLayoutDesc.push_back(elementDesc);
	}

	const void* code = effectVsDesc->pBytecode;
	UINT codeSize = effectVsDesc->BytecodeLength;

	if (inputLayoutDesc.size() > 0)
	{
		ComPtr<ID3D11InputLayout> inputLayout;

		HRESULT hr = Global::g_device->CreateInputLayout
		(
			&inputLayoutDesc[0]
			, inputLayoutDesc.size()
			, code
			, codeSize
			, inputLayout.GetAddressOf()
		);

		if (FAILED(hr))
			ShowErrorMessage(hr);

		return inputLayout;
	}

	return nullptr;
}

void Shader::Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation)
{
	techniques[technique].passes[pass].Draw(vertexCount, startVertexLocation);
}

void Shader::DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	techniques[technique].passes[pass].DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Shader::DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	techniques[technique].passes[pass].DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void Shader::DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	techniques[technique].passes[pass].DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Shader::Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z)
{
	techniques[technique].passes[pass].Dispatch(x, y, z);
}

ComPtr<ID3DX11EffectVariable> Shader::GetVariable(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str());
}

ComPtr<ID3DX11EffectScalarVariable> Shader::GetScalar(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsScalar();
}

ComPtr<ID3DX11EffectVectorVariable> Shader::GetVector(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsVector();
}

ComPtr<ID3DX11EffectMatrixVariable> Shader::GetMatrix(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsMatrix();
}

ComPtr<ID3DX11EffectStringVariable> Shader::GetString(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsString();
}

ComPtr<ID3DX11EffectShaderResourceVariable> Shader::GetSRV(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsShaderResource();
}

ComPtr<ID3DX11EffectRenderTargetViewVariable> Shader::GetRTV(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsRenderTargetView();
}

ComPtr<ID3DX11EffectDepthStencilViewVariable> Shader::GetDSV(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencilView();
}

ComPtr<ID3DX11EffectConstantBuffer> Shader::GetConstantBuffer(std::string name)
{
	return shaderDesc.effect->GetConstantBufferByName(name.c_str());
}

ComPtr<ID3DX11EffectShaderVariable> Shader::GetShader(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsShader();
}

ComPtr<ID3DX11EffectBlendVariable> Shader::GetBlend(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsBlend();
}

ComPtr<ID3DX11EffectDepthStencilVariable> Shader::GetDepthStencil(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencil();
}

ComPtr<ID3DX11EffectRasterizerVariable> Shader::GetRasterizer(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsRasterizer();
}

ComPtr<ID3DX11EffectSamplerVariable> Shader::GetSampler(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsSampler();
}

ComPtr<ID3DX11EffectUnorderedAccessViewVariable> Shader::GetUAV(std::string name)
{
	return shaderDesc.effect->GetVariableByName(name.c_str())->AsUnorderedAccessView();
}


