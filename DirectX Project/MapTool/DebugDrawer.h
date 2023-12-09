#pragma once

class DebugDrawer
{
	std::shared_ptr<Shader> shader;

	std::vector<std::shared_ptr<VertexBuffer>> vertexBufferList;
	std::vector<std::shared_ptr<IndexBuffer>> indexBufferList;

	int pass = 0;
public:
	DebugDrawer();
	~DebugDrawer();

	void DrawLine(Vector3 start, Vector3 end, Color color);
	void DrawBox(Cube box, Color color);
	void DrawRect(std::vector<Vector3>& points, Color color);

	void Update();
	void Render();
};

