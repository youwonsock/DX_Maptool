#include "pch.h"
#include "ShaderRes.h"

#include "InputLayout.h"
#include "ShaderBase.h"

ShaderRes::ShaderRes() : ResourceBase(ResourceType::Shader)
{
}

ShaderRes::~ShaderRes()
{
}

void ShaderRes::SetInputLayout(const std::shared_ptr<InputLayout>& inputLayout)
{
	this->inputLayout = inputLayout;
}

void ShaderRes::SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader)
{
	this->vertexShader = vertexShader;
}

void ShaderRes::SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader)
{
	this->pixelShader = pixelShader;
}

const std::shared_ptr<InputLayout> ShaderRes::GetInputLayout() const
{
	return inputLayout;
}

const std::shared_ptr<VertexShader> ShaderRes::GetVertexShader() const
{
	return vertexShader;
}

const std::shared_ptr<PixelShader> ShaderRes::GetPixelShader() const
{
	return pixelShader;
}
