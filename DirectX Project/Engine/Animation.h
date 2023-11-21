#pragma once

#include "ResourceBase.h"
#include "Struct.h"

class Texture;

class Animation : public ResourceBase
{
private:
	using Base = ResourceBase;

	bool isLooping = false;
	std::shared_ptr<Texture> texture;
	std::vector<KeyFrame> keyFrames;

public:
	Animation();
	~Animation();

	void Save(const std::wstring& path) override;
	void Load(const std::wstring& path) override;

	void SetIsLooping(bool val) { isLooping = val; };
	bool GetIsLooping() const { return isLooping; };

	void SetTexture(const std::shared_ptr<Texture>& texture) { this->texture = texture; };
	const std::shared_ptr<Texture> GetTexture() const { return texture; };
	Vector2& GetTextureSize();

	const KeyFrame& GetKeyFrame(UINT index) const;
	UINT GetKeyFrameCount() const { return keyFrames.size(); };
	void AddKeyFrame(const KeyFrame& keyFrame);
};

