#include "pch.h"
#include "ShaderManager.h"

ShaderDesc ShaderManager::GetEffect(std::wstring fileName)
{
	if (shaders.count(fileName) == 0)
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error;

		//INT flag = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;							// base code 
		INT flag =  D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;	// change to this code
		// D3DCOMPILE_ENABLE_STRICTNESS : strict mode is not supported in legacy shader so not compatible with D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY
		// D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY : enable legacy shader
		// D3DCOMPILE_PACK_MATRIX_ROW_MAJOR : row major matrix

		#ifdef _DEBUG
			flag |= D3DCOMPILE_DEBUG;
			flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
		#endif

		HRESULT hr = ::D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0", flag, NULL, blob.GetAddressOf(), error.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != NULL)
			{
				std::string str = (const char*)error->GetBufferPointer();
				MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}

		ComPtr<ID3DX11Effect> effect;
		hr = ::D3DX11CreateEffectFromMemory(blob->GetBufferPointer(), blob->GetBufferSize(), 0, Global::g_device.Get(), effect.GetAddressOf());
		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);

		shaders[fileName] = ShaderDesc{ blob, effect };
	}

	ShaderDesc desc = shaders.at(fileName);
	ComPtr<ID3DX11Effect> effect;
	desc.effect->CloneEffect(D3DX11_EFFECT_CLONE_FORCE_NONSINGLE, effect.GetAddressOf());

	return ShaderDesc{ desc.blob, effect };
}
