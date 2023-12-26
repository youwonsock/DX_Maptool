#include "pch.h"
#include "ModelRenderer.h"

#include "Material.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Transform.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstancingBuffer.h"

ModelRenderer::ModelRenderer(std::shared_ptr<Shader> shader) : Component(ComponentType::ModelRenderer), shader(shader)
{
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::SetModel(std::shared_ptr<Model> model)
{
	this->model = model;

	const auto& materials = model->GetMaterials();
	for (const auto& material : materials)
	{
		material->SetShader(shader);
	}
}

InstanceID ModelRenderer::GetInstanceID() const
{
	return std::make_pair((UINT)model.get(), (UINT)shader.get());
}

void ModelRenderer::RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer)
{
	if (model == nullptr)
		return;

	// Bones
	BoneDesc boneDesc;

	const UINT boneCount = model->GetBoneCount();

	for (UINT i = 0; i < boneCount; i++)
	{
		std::shared_ptr<ModelBone> bone = model->GetBoneByIndex(i);

		boneDesc.transforms[i] = bone->transform;
	}
	RenderManager::GetInstance().PushBoneData(boneDesc);

	const auto& meshes = model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// BoneIndex
		shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
		 
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		instancingBuffer->PushData();

		shader->DrawIndexedInstanced(0, pass, mesh->indexBuffer->GetIndexCount(), instancingBuffer->GetDataCount());
	}
}

void ModelRenderer::Render()
{
	if (model == nullptr)
		return;

	// Bones
	BoneDesc boneDesc;

	const UINT boneCount = model->GetBoneCount();
	for (UINT i = 0; i < boneCount; i++)
	{
		std::shared_ptr<ModelBone> bone = model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	RenderManager::GetInstance().PushBoneData(boneDesc);

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	RenderManager::GetInstance().PushTransformData(TransformDesc{ world });

	const auto& meshes = model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// BoneIndex
		shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		UINT stride = mesh->vertexBuffer->GetStride();
		UINT offset = mesh->vertexBuffer->GetOffset();

		Global::g_immediateContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		Global::g_immediateContext->IASetIndexBuffer(mesh->indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		shader->DrawIndexed(0, pass, mesh->indexBuffer->GetIndexCount(), 0, 0);
	}
}
