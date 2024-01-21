#pragma once

#include "Engine\RenderTarget.h"

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

