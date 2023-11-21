#include "pch.h"
#include "ShaderBase.h"

void ShaderBase::LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version)
{
	this->path = path;
	this->name = name;

	const UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	if (FAILED(hr))
		ShowErrorMessage(hr);
}

void VertexShader::Create(const std::wstring& path, const std::string& name, const std::string& version)
{
	LoadShaderFromFile(path, name, version);

	HRESULT hr = Global::g_device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		vertexShader.GetAddressOf()
	);

	if (FAILED(hr))
		ShowErrorMessage(hr);
}

void PixelShader::Create(const std::wstring& path, const std::string& name, const std::string& version)
{
	LoadShaderFromFile(path, name, version);

	HRESULT hr = Global::g_device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		pixelShader.GetAddressOf()
	);

	if (FAILED(hr))
		ShowErrorMessage(hr);
}
