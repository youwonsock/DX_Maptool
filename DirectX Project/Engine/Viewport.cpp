#include "pch.h"
#include "Viewport.h"

void Viewport::RSSetViewport()
{
	Global::g_immediateContext->RSSetViewports(1, &viewport);
}

void Viewport::SetViewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
}

Vector3 Viewport::Project(const Vector3& pos, const Matrix& world, const Matrix& view, const Matrix& projection)
{
	Matrix wvp = world * view * projection;
	Vector3 p = Vector3::Transform(pos, wvp);

	p.x = (p.x + 1.0f) * (viewport.Width / 2) + viewport.TopLeftX;
	p.y = (-p.y + 1.0f) * (viewport.Height / 2) + viewport.TopLeftY;
	p.z = p.z * (viewport.MaxDepth - viewport.MinDepth) + viewport.MinDepth;

	return p;
}

Vector3 Viewport::Unproject(const Vector3& pos, const Matrix& world, const Matrix& view, const Matrix& projection)
{
	Vector3 p = pos;

	p.x = 2.f * (p.x - viewport.TopLeftX) / viewport.Width - 1.f;
	p.y = -2.f * (p.y - viewport.TopLeftY) / viewport.Height + 1.f;
	p.z = ((p.z - viewport.MinDepth) / (viewport.MaxDepth - viewport.MinDepth));

	Matrix wvp = world * view * projection;
	Matrix wvpInv = wvp.Invert();

	p = Vector3::Transform(p, wvpInv);
	return p;
}

Viewport::Viewport()
{
    SetViewport(800, 600);
}

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	SetViewport(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
}
