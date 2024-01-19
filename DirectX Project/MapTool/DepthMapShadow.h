#pragma once
class DepthMapShadow
{
public:
	Matrix lightView;
	Matrix lightProj;

public:
	DepthMapShadow();
	~DepthMapShadow();

	void Init();
	void Update();
	void Render();
};

