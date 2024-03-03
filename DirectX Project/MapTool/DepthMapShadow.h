#pragma once

#include "Engine\RenderTarget.h"

// renderTarget�� �߿����� �������� �����ʹϱ� ȯ? �� Renderer(Singleton)�� ����� �ű⼭ light ���� ���� ��������

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

