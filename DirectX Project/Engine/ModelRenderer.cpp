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

#include "ModelAnimation.h"
#include "GameObject.h"
#include "Figure.h"

ModelRenderer::ModelRenderer(std::shared_ptr<Shader> shader) : Component(ComponentType::ModelRenderer), shader(shader)
{
	keyframeDesc.speed = rand() % 2 + 1;
	keyframeDesc.currentFrame = rand() % 40;
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::SetModel(std::shared_ptr<Model> model)
{
	this->model = model;

	this->owner.lock()->GetTransform()->SetDefaultBoundingBox(model->GetBoundingBox());

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

	if (pass == 1) // use animation
	{
		if (texture == nullptr)
			CreateTexture();

		UpdateKeyframeDesc();
		RenderManager::GetInstance().PushKeyframeData(keyframeDesc);

		shader->GetSRV("TransformMap")->SetResource(textureSRV.Get());
	}

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

	if (pass == 1) // use animation
	{
		if (texture == nullptr)
			CreateTexture();

		UpdateKeyframeDesc();
		RenderManager::GetInstance().PushKeyframeData(keyframeDesc);

		shader->GetSRV("TransformMap")->SetResource(textureSRV.Get());
	}

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

void ModelRenderer::UpdateKeyframeDesc()
{
	KeyframeDesc& desc = keyframeDesc;

	desc.sumTime += TimeManager::GetInstance().GetDeltaTime();

	std::shared_ptr<ModelAnimation> currentAnim = model->GetAnimationByIndex(desc.animIdx);
	if (currentAnim)
	{
		float timePerFrame = 1 / (currentAnim->frameRate * desc.speed * 0.5f);

		if (desc.sumTime >= timePerFrame)
		{
			desc.sumTime = 0;
			desc.currentFrame = (desc.currentFrame + 1) % currentAnim->frameCount;

			if (currentAnim->frameCount < desc.currentFrame)
				desc.currentFrame = 0;
		}

		desc.ratio = (desc.sumTime / timePerFrame);
	}
}

void ModelRenderer::CreateTexture()
{
	if (model->GetAnimationCount() == 0)
		return;

	animTransforms.resize(model->GetAnimationCount());
	for (UINT i = 0; i < model->GetAnimationCount(); ++i)
		CreateAnimationTransform(i);

	//create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4; // matrix 4x4 = 64 bytes
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = model->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16 bytes
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		const UINT dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const UINT pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = malloc(pageSize * model->GetAnimationCount());

		for (UINT c = 0; c < model->GetAnimationCount(); ++c)
		{
			UINT startOffset = c * pageSize;
			BYTE* pageStartPtr = static_cast<BYTE*>(mallocPtr) + startOffset;

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; ++i)
			{
				void* ptr = pageStartPtr + dataSize * i;
				memcpy(ptr, animTransforms[c].transforms[i].data(), dataSize);
			}
		}

		std::vector<D3D11_SUBRESOURCE_DATA> subResources(model->GetAnimationCount());

		for (UINT i = 0; i < model->GetAnimationCount(); ++i)
		{
			void* ptr = (BYTE*)mallocPtr + (pageSize * i);
			subResources[i].pSysMem = ptr;
			subResources[i].SysMemPitch = dataSize;
			subResources[i].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = Global::g_device->CreateTexture2D(&desc, subResources.data(), texture.GetAddressOf());
		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);

		free(mallocPtr);
	}

	//create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->GetAnimationCount();

		HRESULT hr = Global::g_device->CreateShaderResourceView(texture.Get(), &desc, textureSRV.GetAddressOf());
		if (FAILED(hr))
			Utils::ShowErrorMessage(hr);
	}
}

void ModelRenderer::CreateAnimationTransform(UINT index)
{
	std::shared_ptr<ModelAnimation> animation = model->GetAnimationByIndex(index);

	for (UINT i = 0; i < animation->frameCount; ++i)
	{
		for (UINT j = 0; j < model->GetBoneCount(); ++j)
		{
			std::shared_ptr<ModelBone> bone = model->GetBoneByIndex(j);
			Matrix matAnimation;

			std::shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
			if (frame != nullptr)
			{
				ModelKeyframeData& data = frame->transforms[i];

				Matrix S, R, T;

				S = Matrix::CreateScale(data.scale);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation);

				matAnimation = S * R * T;
			}
			else
			{
				matAnimation = Matrix::Identity;
			}

			animTransforms[index].transforms[i][j] = matAnimation;
		}
	}
}