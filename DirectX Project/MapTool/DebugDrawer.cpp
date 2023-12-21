#include "pch.h"
#include "DebugDrawer.h"

DebugDrawer::DebugDrawer()
{
	shader = std::make_shared<Shader>(L"MapToolShader/DebugShader.fx");
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::DrawLine(Vector3 start, Vector3 end, Color color)
{
	std::vector<PNCTVertex> vertices;
	vertices.resize(2);

	vertices[0].position = start;
	vertices[0].color = color;
	vertices[1].position = end;
	vertices[1].color = color;

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vertices);
	vertexBufferList.push_back(vertexBuffer);

	std::vector<UINT> indices;
	indices.resize(2);
	indices[0] = 0;
	indices[1] = 1;
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(indices);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::DrawBox(Cube box, Color color)
{
	std::vector<PNCTVertex> vtx(24);

	

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	//  0   1   4   5
	//
	//  2   3   6   7
	Vector3 c0, c1, c2, c3, c4, c5, c6, c7;
	Vector3 size = box.size;

	c0 = box.min; c0.y += size.y;
	c1 = box.min; c1.x += size.x; c1.y += size.y;
	c2 = box.min;
	c3 = box.min; c3.x += size.x;
	c4 = box.min; c4.z += size.z; c4.y += size.y;
	c5 = box.max;
	c6 = box.min; c6.z += size.z;
	c7 = box.max; c7.y -= size.y;

	// ¾Õ¸é
	vtx[0] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[1] = PNCTVertex(c1, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[2] = PNCTVertex(c3, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[3] = PNCTVertex(c2, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// µÞ¸é
	vtx[4] = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[5] = PNCTVertex(c5, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[6] = PNCTVertex(c7, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[7] = PNCTVertex(c6, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// À­¸é
	vtx[8]  = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[9]  = PNCTVertex(c5, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[10] = PNCTVertex(c1, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[11] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// ¾Æ·§¸é
	vtx[12] = PNCTVertex(c2, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[13] = PNCTVertex(c3, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[14] = PNCTVertex(c7, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[15] = PNCTVertex(c6, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// ¿ÞÂÊ¸é
	vtx[16] = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[17] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[18] = PNCTVertex(c2, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[19] = PNCTVertex(c6, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// ¿À¸¥ÂÊ¸é
	vtx[20] = PNCTVertex(c1, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[21] = PNCTVertex(c5, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[22] = PNCTVertex(c7, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[23] = PNCTVertex(c3, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vtx);
	vertexBufferList.push_back(vertexBuffer);



	std::vector<UINT> idx(36);
	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µÞ¸é
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// À­¸é
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// ¾Æ·§¸é
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ¿ÞÂÊ¸é
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ¿À¸¥ÂÊ
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(idx);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::DrawRect(std::vector<Vector3>& points, Color color)
{
	std::vector<PNCTVertex> vtx(4);

	vtx[0] = PNCTVertex(points[0], Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[1] = PNCTVertex(points[1], Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[2] = PNCTVertex(points[2], Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[3] = PNCTVertex(points[3], Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vtx);
	vertexBufferList.push_back(vertexBuffer);

	std::vector<UINT> idx(6);
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 2; idx[4] = 1; idx[5] = 3;

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(idx);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::Update()
{
	ImGui::InputInt("Pass", &pass);

	if (pass < 0)
		pass = 0;

	if (pass > 1)
		pass = 1;

	if (InputManager::GetInstance().GetKeyState(DIK_I) == KeyState::PUSH)
	{
		vertexBufferList.clear();
		indexBufferList.clear();
	}
}

void DebugDrawer::Render()
{
	Matrix world = Matrix::Identity;
	shader->GetMatrix("World")->SetMatrix((float*)&world);
	shader->GetMatrix("View")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->viewMatrix);
	shader->GetMatrix("Projection")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->projectionMatrix);

	for (int i = 0; i < vertexBufferList.size(); i++)
	{
		UINT stride = vertexBufferList[i]->GetStride();
		UINT offset = vertexBufferList[i]->GetOffset();

		Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBufferList[i]->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		Global::g_immediateContext->IASetIndexBuffer(indexBufferList[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		shader->DrawIndexed(0, pass, indexBufferList[i]->GetIndexCount());
	}
}
