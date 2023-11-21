#pragma once

#include "ResourceBase.h"

class InputLayout;
class VertexShader;
class PixelShader;

class ShaderRes : public ResourceBase
{
private:
	using Base = ResourceBase;

	std::shared_ptr<InputLayout> inputLayout;
	std::shared_ptr<VertexShader> vertexShader;
	std::shared_ptr<PixelShader> pixelShader;

public:
	ShaderRes();
	~ShaderRes();

	void SetInputLayout(const std::shared_ptr<InputLayout>& inputLayout);
	void SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader);
	void SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader);

	const std::shared_ptr<InputLayout> GetInputLayout() const;
	const std::shared_ptr<VertexShader> GetVertexShader() const;
	const std::shared_ptr<PixelShader> GetPixelShader() const;
};

