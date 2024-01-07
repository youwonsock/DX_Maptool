 #include "pch.h"
#include "GroupDemo.h"

#include "Engine/TestCamera.h"

void GroupDemo::Init()
{
	shader = std::make_shared<Shader>(L"25. GroupDemo.fx");

	// 하나의 쓰레드 그룹에서 운영할 쓰레드 개수
	UINT threadCount = 10 * 8 * 3;
	UINT groupCount = 2 * 1 * 1;
	UINT count = threadCount * groupCount;

	std::vector<Input> inputs(count);
	for(UINT i = 0; i < count; i++)
		inputs[i].value = rand() % 10000;

	std::shared_ptr<RawBuffer> buffer = std::make_shared<RawBuffer>(inputs.data(), sizeof(Input) * count, sizeof(Output) * count);

	shader->GetSRV("Input")->SetResource(buffer->GetInputSRV().Get());
	shader->GetUAV("Output")->SetUnorderedAccessView(buffer->GetOutputUAV().Get());
	
	// x, y, z : 쓰레드 그룹
	shader->Dispatch(0,0,2,1,1); // compute shader 실행 (그룹 2개, 그룹 당 쓰레드 240개)

	std::vector<Output> outputs(count);
	buffer->CopuFromOutput(outputs.data());

	int a = 0;
}
void GroupDemo::FixedUpdate()
{
}

void GroupDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}
}

void GroupDemo::PostUpdate()
{
}

void GroupDemo::PreRender()
{
}

void GroupDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GroupDemo::PostRender()
{
}

void GroupDemo::Release()
{
}
