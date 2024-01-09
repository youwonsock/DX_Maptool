#pragma once
class Viewport
{
private:
	D3D11_VIEWPORT viewport;

public:
	void RSSetViewport();
	void SetViewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return viewport.Width; }
	float GetHeight() { return viewport.Height; }

	Vector3 Project(const Vector3& pos, const Matrix& world, const Matrix& view, const Matrix& projection);
	Vector3 Unproject(const Vector3& pos, const Matrix& world, const Matrix& view, const Matrix& projection);

	Viewport();
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

};

