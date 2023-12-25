#include "pch.h"
#include "ModelAnimator.h"

#include "ModelMesh.h"
#include "Material.h"
#include "Model.h"
#include "ModelAnimation.h"
#include "Shader.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "InstancingBuffer.h"

ModelAnimator::ModelAnimator(std::shared_ptr<Shader> shader) : Base(ComponentType::Animator), shader(shader)
{
	tweenDesc.next.animIdx = rand() % 3;
	tweenDesc.tweenSumTime += rand() % 100;
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::CreateTexture()
{
	if(model->GetAnimationCount() == 0)
		return;	

	animTransforms.resize(model->GetAnimationCount());
	for(UINT i = 0; i< model->GetAnimationCount(); ++i)
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

void ModelAnimator::CreateAnimationTransform(UINT index)
{
	std::vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);
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

			Matrix toRootMat = bone->transform;
			Matrix invGlobal = toRootMat.Invert();

			int parentIndex = bone->parentIndex;

			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransforms[parentIndex];

			tempAnimBoneTransforms[j] = matAnimation * matParent;

			animTransforms[index].transforms[i][j] = invGlobal * tempAnimBoneTransforms[j];
		}
	}
}

void ModelAnimator::SetModel(std::shared_ptr<Model> model)
{
	this->model = model;

	const auto& materials = model->GetMaterials();

	for (auto& material : materials)
		material->SetShader(shader);
}

InstanceID ModelAnimator::GetInstanceID() const
{
	return std::make_pair((UINT)model.get(), (UINT)shader.get());
}

void ModelAnimator::RenderInstancing(std::shared_ptr<class InstancingBuffer>& instancingBuffer)
{
	if (model == nullptr)
		return;
	if (texture == nullptr)
		CreateTexture();


	shader->GetSRV("TransformMap")->SetResource(textureSRV.Get());

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

void ModelAnimator::UpdateTweenData()
{
	TweenDesc& desc = tweenDesc;

	desc.curr.sumTime += TimeManager::GetInstance().GetDeltaTime();

	{
		std::shared_ptr<ModelAnimation> currentAnim = model->GetAnimationByIndex(desc.curr.animIdx);
		if (currentAnim)
		{
			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);

			// for random speed
			timePerFrame *= (rand() % 10);

			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;
				desc.curr.currentFrame = (desc.curr.currentFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currentFrame + 1) % currentAnim->frameCount;
			}

			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	if (desc.next.animIdx >= 0)
	{
		desc.tweenSumTime += TimeManager::GetInstance().GetDeltaTime();
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			desc.curr = desc.next;
			desc.ClearNextAnim();
		}
		else
		{
			std::shared_ptr<ModelAnimation> nextAnim = model->GetAnimationByIndex(desc.next.animIdx);
			desc.next.sumTime += TimeManager::GetInstance().GetDeltaTime();

			float frameRate;
			UINT frameCount;

			if (nextAnim.get() == NULL)
			{
				desc.ClearNextAnim();

				frameRate = 1.f;
				frameCount = 1;
			}
			else
			{
				frameRate = nextAnim->frameRate;
				frameCount = nextAnim->frameCount;
			}

			float timePerFrame = 1.f / (frameRate * desc.next.speed);

			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;

				desc.next.currentFrame = (desc.next.currentFrame + 1) % frameCount;
				desc.next.nextFrame = (desc.next.currentFrame + 1) % frameCount;
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}

}
