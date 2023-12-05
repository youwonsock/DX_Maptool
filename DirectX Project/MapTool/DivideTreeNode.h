#pragma once

#include "Engine/Figure.h"

struct DivideTreeNode
{
	std::vector<std::shared_ptr<DivideTreeNode>> children;

	Cube boundingBox;

	UINT width = 0;
	UINT height = 0;
	int depth = 0;

	bool isLeaf = false;

	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	void CreateVertexBuffer(std::vector<PNCTVertex>& verteces);
	void Update();
	void Render();
};

