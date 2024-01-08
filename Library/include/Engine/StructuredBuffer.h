#pragma once
class StructuredBuffer
{
private:
	void* inputData;
	UINT inputStride = 0;
	UINT inputCount = 0;
	UINT outputStride = 0;
	UINT outputCount = 0;

	ComPtr<ID3D11Buffer> input;
	ComPtr<ID3D11ShaderResourceView> inputSRV;
	ComPtr<ID3D11Buffer> output;
	ComPtr<ID3D11UnorderedAccessView> outputUAV;
	ComPtr<ID3D11Buffer> result;

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	void CreateBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride = 0, UINT outputCount = 0);

	void CopyToInput(void* data);
	void CopuFromOutput(void* data);

	ComPtr<ID3D11ShaderResourceView> GetInputSRV() { return inputSRV; }
	ComPtr<ID3D11UnorderedAccessView> GetOutputUAV() { return outputUAV; }
	UINT GetInputByteWidth() { return inputStride * inputCount; }
	UINT GetOutputByteWidth() { return outputStride * outputCount; }

	StructuredBuffer();
	~StructuredBuffer();
};

