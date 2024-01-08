#pragma once
class TextureBuffer
{
private:
	ComPtr<ID3D11Texture2D> input;
	ComPtr<ID3D11ShaderResourceView> srv; // Input
	ComPtr<ID3D11Texture2D> output;
	ComPtr<ID3D11UnorderedAccessView> uav; // Output

	UINT width = 0;
	UINT height = 0;
	UINT arraySize = 0;
	DXGI_FORMAT format;
	ComPtr<ID3D11ShaderResourceView> outputSRV;

public:
	void CreateBuffer(ComPtr<ID3D11Texture2D> src);

private:
	void CreateInput(ComPtr<ID3D11Texture2D> src);
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	UINT GetWidth() { return width; }
	UINT GetHeight() { return height; }
	UINT GetArraySize() { return arraySize; }

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUnorderedAccessView() { return uav; }
	ComPtr<ID3D11ShaderResourceView> GetOutputSRV() { return outputSRV; }
	ComPtr<ID3D11Texture2D> GetOutput() { return (ID3D11Texture2D*)output.Get(); }

	TextureBuffer();
	~TextureBuffer();
};

