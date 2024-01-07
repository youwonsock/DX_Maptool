 #include "pch.h"
#include "RawBufferDemo.h"

#include "Engine/TestCamera.h"

void RawBufferDemo::Init()
{
	shader = std::make_shared<Shader>(L"24. RawBufferDemo.fx");

	// �ϳ��� ������ �׷쿡�� ��� ������ ����
	UINT count = 10 * 8 * 3;
	std::shared_ptr<RawBuffer> buffer = std::make_shared<RawBuffer>(nullptr, 0, sizeof(Output) * count);

	shader->GetUAV("Output")->SetUnorderedAccessView(buffer->GetOutputUAV().Get());
	
	// x, y, z : ������ �׷�
	shader->Dispatch(0,0,1,1,1); // compute shader ����

	std::vector<Output> outputs(count);
	buffer->CopuFromOutput(outputs.data());

	int a = 0;
}
void RawBufferDemo::FixedUpdate()
{
}

void RawBufferDemo::Update()
{
	//show FPS
	{
		std::wstring str = std::to_wstring(TimeManager::GetInstance().GetFPS());
		SetWindowText(Global::g_hWnd, str.c_str());
	}
}

void RawBufferDemo::PostUpdate()
{
}

void RawBufferDemo::PreRender()
{
}

void RawBufferDemo::Render()
{
	Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RawBufferDemo::PostRender()
{
}

void RawBufferDemo::Release()
{
}
