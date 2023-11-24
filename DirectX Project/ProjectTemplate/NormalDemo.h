#pragma once

#include "Engine/IExecute.h"

#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

#include "Engine/EngineHeader.h"
#include "Engine/PipeLineHeaders.h"
#include "Engine/ResourceHeader.h"

class NormalDemo : public IExecute
{
private:
	std::shared_ptr<Shader> shader;

	std::shared_ptr<Texture> texture;
	std::shared_ptr<Texture> heightMap;

	std::shared_ptr<Geometry<VertexTextureNormalData>> geometry;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	Matrix world = Matrix::Identity;

	//camera
	std::shared_ptr<GameObject> cameraObject;

	// light dir
	Vector3 lightDir = Vector3(0, -1, 0);
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

