#pragma once
class RawBuffer
{
private:
	void* inputData;
	UINT inputByte = 0;
	UINT outputByte = 0;

	ComPtr<ID3D11Buffer> input;
	ComPtr<ID3D11ShaderResourceView> inputSRV;
	ComPtr<ID3D11Buffer> output;
	ComPtr<ID3D11UnorderedAccessView> outputUAV;
	ComPtr<ID3D11Buffer> result;

public:

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	void CreateBuffer();
	void CopyToInput(void* data);
	void CopuFromOutput(void* data);

	ComPtr<ID3D11ShaderResourceView> GetInputSRV() { return inputSRV; }
	ComPtr<ID3D11UnorderedAccessView> GetOutputUAV() { return outputUAV; }
	
	RawBuffer(void* inputData, UINT inputByte, UINT outputByte);
	~RawBuffer();
};

