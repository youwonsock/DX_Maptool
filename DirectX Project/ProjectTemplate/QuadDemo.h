#pragma once

#include "Engine/IExecute.h"

#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

#include "Engine/EngineHeader.h"
#include "Engine/PipeLineHeaders.h"
#include "Engine/ResourceHeader.h"

class QuadDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Geometry<VertexColorData>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

public:
	// IExecute을(를) 통해 상속됨
	void Init() override;
	void FixedUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Release() override;
};

