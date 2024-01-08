 #include "pch.h"
#include "StructredBufferDemo.h"

#include "Engine/TestCamera.h"

void StructredBufferDemo::Init()
{
	shader = std::make_shared<Shader>(L"27. StructuredBufferDemo.fx");

	auto t1 = std::chrono::high_resolution_clock::now();

	std::vector<Matrix> inputs(500, Matrix::Identity);
	std::vector<Matrix> outputs(500);

	// use gpu
	{
		auto buffer = std::make_shared<StructuredBuffer>();
		buffer->CreateBuffer(inputs.data(), sizeof(Matrix), 500, sizeof(Matrix), 500);

		shader->GetSRV("Input")->SetResource(buffer->GetInputSRV().Get());
		shader->GetUAV("Output")->SetUnorderedAccessView(buffer->GetOutputUAV().Get());

		shader->Dispatch(0, 0, 1, 1, 1);

		buffer->CopuFromOutput(outputs.data());
	}

	auto endtime = std::chrono::high_resolution_clock::now() - t1;

	int a = 10;
}
void StructredBufferDemo::FixedUpdate() 
{
}

void StructredBufferDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}
}

void StructredBufferDemo::PostUpdate()
{
}

void StructredBufferDemo::PreRender()
{
}

void StructredBufferDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void StructredBufferDemo::PostRender()
{
}

void StructredBufferDemo::Release()
{
}
