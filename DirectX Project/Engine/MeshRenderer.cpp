#include "pch.h"
#include "MeshRenderer.h"

#include "ResourceHeader.h"
#include "Transform.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

MeshRenderer::MeshRenderer() : Base(ComponentType::MeshRenderer)
{
}
MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}
void MeshRenderer::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh() const
{
	return mesh;
}
std::shared_ptr<Material> MeshRenderer::GetMaterial() const
{
	return material;
}

void MeshRenderer::Update()
{
	if (mesh == nullptr || material == nullptr)
		return;

	auto shader = material->GetShader();
	if (shader == nullptr)
		return;

	material->Update();

	auto world = GetTransform()->GetWorldMatrix();
	RenderManager::GetInstance().PushTransformData(TransformDesc{ world });

	UINT stride = mesh->GetVertexBuffer()->GetStride();
	UINT offset = mesh->GetIndexBuffer()->GetOffset();

	Global::g_immediateContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	Global::g_immediateContext->IASetIndexBuffer(mesh->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}