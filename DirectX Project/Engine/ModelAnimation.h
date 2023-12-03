#pragma once

struct ModelKeyframeData
{
	float time;
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};

struct ModelKeyframe
{
	std::wstring boneName;
	std::vector<ModelKeyframeData> transforms;
};

struct ModelAnimation
{
	std::shared_ptr<ModelKeyframe> GetKeyframe(std::wstring boneName);

	std::wstring name;
	float duration = 0.f;
	float frameRate = 0.f;
	UINT frameCount = 0;
	std::unordered_map<std::wstring, std::shared_ptr<ModelKeyframe>> keyframes;
};

