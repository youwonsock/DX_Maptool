#pragma once

class ShaderBase
{
protected:
	ComPtr<ID3DBlob> blob;
	std::wstring path;
	std::string name;

protected:
	void LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version);

public:
	ShaderBase() {};
	virtual ~ShaderBase() {};

	ComPtr<ID3DBlob> GetBlob() { return blob; }

	virtual void Create(const std::wstring& path, const std::string& name, const std::string& version) abstract;
};

class VertexShader : public ShaderBase
{
protected:
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;

public:
	VertexShader() {};
	virtual ~VertexShader() {};

	ComPtr<ID3D11VertexShader> GetShader() { return vertexShader; }

	virtual void Create(const std::wstring& path, const std::string& name, const std::string& version) override;
};

class PixelShader : public ShaderBase
{
protected:
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;

public:
	PixelShader() {};
	virtual ~PixelShader() {};

	ComPtr<ID3D11PixelShader> GetShader() { return pixelShader; }

	virtual void Create(const std::wstring& path, const std::string& name, const std::string& version) override;
};