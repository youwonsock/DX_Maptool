#include "pch.h"
#include "Shader.h"

#include "InputLayout.h"
#include "ShaderBase.h"

Shader::Shader() : ResourceBase(ResourceType::Shader)
{
}

Shader::~Shader()
{
}

void Shader::SetInputLayout(const std::shared_ptr<InputLayout>& inputLayout)
{
	this->inputLayout = inputLayout;
}

void Shader::SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader)
{
	this->vertexShader = vertexShader;
}

void Shader::SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader)
{
	this->pixelShader = pixelShader;
}

const std::shared_ptr<InputLayout> Shader::GetInputLayout() const
{
	return inputLayout;
}

const std::shared_ptr<VertexShader> Shader::GetVertexShader() const
{
	return vertexShader;
}

const std::shared_ptr<PixelShader> Shader::GetPixelShader() const
{
	return pixelShader;
}
