#include "pch.h"
#include "MeshRenderer.h"

#include "ResourceHeader.h"
#include "Transform.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "InstancingBuffer.h"

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

void MeshRenderer::SetPass(UINT pass)
{
	this->pass = pass;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh() const
{
	return mesh;
}
std::shared_ptr<Material> MeshRenderer::GetMaterial() const
{
	return material;
}

UINT MeshRenderer::GetPass() const
{
	return pass;
}

InstanceID MeshRenderer::GetInstanceID() const
{
	return std::make_pair((UINT)mesh.get(), (UINT)material.get());
}

void MeshRenderer::RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer)
{
	if (mesh == nullptr || material == nullptr)
		return;

	auto shader = material->GetShader();
	if (shader == nullptr)
		return;

	material->Update();

	mesh->GetVertexBuffer()->PushData();
	mesh->GetIndexBuffer()->PushData();

	instancingBuffer->PushData();

	shader->DrawIndexedInstanced(0, pass, mesh->GetIndexBuffer()->GetIndexCount(),instancingBuffer->GetDataCount());
}
