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

	c0 = box.points[0];
	c1 = box.points[1]; 
	c2 = box.points[2]; 
	c3 = box.points[3]; 
	c4 = box.points[4]; 
	c5 = box.points[5]; 
	c6 = box.points[6]; 
	c7 = box.points[7]; 

	// �ո�
	vtx[0] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[1] = PNCTVertex(c1, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[2] = PNCTVertex(c3, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[3] = PNCTVertex(c2, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// �޸�
	vtx[4] = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[5] = PNCTVertex(c5, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[6] = PNCTVertex(c7, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[7] = PNCTVertex(c6, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// ����
	vtx[8]  = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[9]  = PNCTVertex(c5, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[10] = PNCTVertex(c1, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[11] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// �Ʒ���
	vtx[12] = PNCTVertex(c2, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[13] = PNCTVertex(c3, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[14] = PNCTVertex(c7, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[15] = PNCTVertex(c6, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// ���ʸ�
	vtx[16] = PNCTVertex(c4, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[17] = PNCTVertex(c0, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[18] = PNCTVertex(c2, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[19] = PNCTVertex(c6, Vector2(0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	// �����ʸ�
	vtx[20] = PNCTVertex(c1, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[21] = PNCTVertex(c5, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[22] = PNCTVertex(c7, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));
	vtx[23] = PNCTVertex(c3, Vector2( 0.0f, 0.0f), color, Vector3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vtx);
	vertexBufferList.push_back(vertexBuffer);



	std::vector<UINT> idx(36);
	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ������
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
	if (InputManager::GetInstance().GetKeyState(DIK_I) == KeyState::PUSH)
	{
		vertexBufferList.clear();
		indexBufferList.clear();
	}
}

void DebugDrawer::Render()
{
	Matrix world = Matrix::Identity;
	shader->GetMatrix("WorldD")->SetMatrix((float*)&world);
	shader->GetMatrix("ViewD")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->viewMatrix);
	shader->GetMatrix("ProjectionD")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->projectionMatrix);

	for (int i = 0; i < vertexBufferList.size(); i++)
	{
		UINT stride = vertexBufferList[i]->GetStride();
		UINT offset = vertexBufferList[i]->GetOffset();

		Global::g_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Global::g_immediateContext->IASetVertexBuffers(0, 1, vertexBufferList[i]->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		Global::g_immediateContext->IASetIndexBuffer(indexBufferList[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		shader->DrawIndexed(0, 0, indexBufferList[i]->GetIndexCount());
	}
}
