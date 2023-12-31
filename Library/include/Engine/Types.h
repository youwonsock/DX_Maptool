#pragma once

using Color = XMFLOAT4;
using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;

using Quaternion = DirectX::SimpleMath::Quaternion;
using Matrix = DirectX::SimpleMath::Matrix;

using InstanceID = std::pair<UINT, UINT>;

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500