#include "pch.h"
#include "MeshRenderer.h"

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

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

std::shared_ptr<Mesh> MeshRenderer::GetMesh() const
{
	return mesh;
}

void MeshRenderer::SetMaterial(std::shared_ptr<Material> meterial)
{
	this->material = meterial;
}

void MeshRenderer::SetShader(std::shared_ptr<Shader> shader)
{
	this->material->SetShader(shader);
}

std::shared_ptr<Material> MeshRenderer::GetMeterial() const
{
	return material;
}

std::shared_ptr<VertexShader> MeshRenderer::GetVertexShader() const
{
	return material->GetShader()->GetVertexShader();
}

std::shared_ptr<PixelShader> MeshRenderer::GetPixelShader() const
{
	return material->GetShader()->GetPixelShader();
}

std::shared_ptr<InputLayout> MeshRenderer::GetInputLayout() const
{
	return material->GetShader()->GetInputLayout();
}

void MeshRenderer::SetTexture(std::shared_ptr<Texture> texture)
{
	this->material->SetTexture(texture);
}

std::shared_ptr<Texture> MeshRenderer::GetTexture() const
{
	return material->GetTexture();
}
