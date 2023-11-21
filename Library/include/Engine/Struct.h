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