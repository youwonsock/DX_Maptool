#pragma once

#include "Types.h"

struct CameraData
{
	Matrix viewMatrix = Matrix::Identity;
	Matrix projectionMatrix = Matrix::Identity;
};

struct TransformData
{
	Matrix worldMatrix = Matrix::Identity;
};

struct AnimationData
{
	Vector2 spriteOffset;
	Vector2 spriteSize;
	Vector2 textureSize;
	float useAnimation;
	float padding;
};

struct KeyFrame
{
	Vector2 offset = Vector2::Zero;
	Vector2 size = Vector2::Zero;
	float time = 0.0f;
};

struct ShaderDesc
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DX11Effect> effect;
};

struct AnimTransform
{
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};
struct GlobalDesc
{
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	Matrix VirwProjection = Matrix::Identity;
	Matrix ViewInverse = Matrix::Identity;
};

struct TransformDesc
{
	Matrix World = Matrix::Identity;
};

struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(1.f, 1.f, 1.f, 1.f);

	Vector3 direction = Vector3(0.f, 0.f, 0.f);
	float pad0;
};

struct MaterialDesc
{
	Color ambient = Color(0.f, 0.f, 0.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(0.f, 0.f, 0.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
};

struct BoneDesc
{
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

struct KeyframeDesc
{
	int animIdx = 0;
	UINT currentFrame = 0;
	UINT nextFrame = 0;
	float ratio = 0.f;
	float sumTime = 0.f;
	float speed = 1.f;
	Vector2 padding;
};

struct TweenDesc
{
	float tweenDuration = 1.f;
	float tweenRatio = 0.f;
	float tweenSumTime = 0.f;
	float padding = 0.f;

	KeyframeDesc curr;
	KeyframeDesc next;

	TweenDesc()
	{
		curr.animIdx = 0;
		next.animIdx = -1;
	}

	void ClearNextAnim()
	{
		next.animIdx = -1;
		next.currentFrame = 0;
		next.nextFrame = 0;
		next.sumTime = 0;
		tweenSumTime = 0;
		tweenRatio = 0;
	}
};

struct InstancedTweenDesc
{
	TweenDesc tweens[MAX_MODEL_INSTANCE];
};

struct InstancedKeyFrameDesc
{
	KeyframeDesc keyframes[MAX_MODEL_INSTANCE];
};