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
	lightView = Matrix::CreateLookAt(Vector3(0, 0, 0), Vector3(1, -1.0f, 1), Vector3(0, 1, 0)); // light direction과 일치하게 생성
	
	float viewDistance = 129 * 1 + 129 * 1; // 129*1 + 129 * 1은 terrain의 전체 크기(모든 오브젝트가 깊이맵에 들어오게 하려고)
	lightProj = Matrix::CreateOrthographic(viewDistance, viewDistance, 0.1f, 100.0f);


	textureMatrix._11 = 0.5f; textureMatrix._22 = -0.5f;
	textureMatrix._41 = 0.5f; textureMatrix._42 = 0.5f;
	renderTarget.Create(1024, 1024);

	shadowMatrix = lightView * lightProj * textureMatrix;


}
