#pragma once

// ExLib.h and StdHeader.h must be included in front of Engine.h.

#include "Engine/IExecute.h"

#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

#include "Engine/Shader.h"
#include "Engine/VertexBuffer.h"
#include "Engine/VertexData.h"

class Sample : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<VertexBuffer> buffer;
	std::vector<VertexData> vertices;

public:
	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Release() override;
};
