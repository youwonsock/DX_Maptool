#pragma once

#include "Engine/IExecute.h"

#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

#include "Engine/EngineHeader.h"
#include "Engine/PipeLineHeaders.h"
#include "Engine/ResourceHeader.h"

class ConstBufferDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Geometry<VertexColorData>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	Vector3 translation = Vector3(0, 0, 0);

	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;

public:
	// IExecute��(��) ���� ��ӵ�
	void Init() override;
	void FixedUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Release() override;
};

