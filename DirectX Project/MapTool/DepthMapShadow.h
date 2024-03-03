#pragma once

#include "Engine\RenderTarget.h"

// renderTarget이 중요하지 나머지는 데이터니까 환? 경 Renderer(Singleton)를 만들어 거기서 light 정보 등을 관리하자

class DepthMapShadow
{
private:

public:

	Matrix lightView;
	Matrix lightProj;
	Matrix textureMatrix;

	Matrix shadowMatrix;

	RenderTarget renderTarget;
public:
	DepthMapShadow();
	~DepthMapShadow();

	void Init();
};

