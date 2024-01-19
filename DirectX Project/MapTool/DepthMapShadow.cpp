#include "pch.h"
#include "DepthMapShadow.h"

DepthMapShadow::DepthMapShadow()
{
}

DepthMapShadow::~DepthMapShadow()
{
}

void DepthMapShadow::Init()
{
	lightView = Matrix::CreateLookAt(Vector3(0, 0, 0), Vector3(1, -1.0f, 1), Vector3(0, 1, 0)); // light direction�� ��ġ�ϰ� ����
	
	float viewDistance = 129 * 1 + 129 * 1; // 129*1 + 129 * 1�� terrain�� ��ü ũ��(��� ������Ʈ�� ���̸ʿ� ������ �Ϸ���)
	lightProj = Matrix::CreateOrthographic(viewDistance, viewDistance, 0.1f, 100.0f);


}

void DepthMapShadow::Update()
{
}

void DepthMapShadow::Render()
{
}
