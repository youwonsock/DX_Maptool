#pragma once

#include "Component.h"

class Animation;

class Animator : public Component
{
private:
	using Base = Component;

	std::shared_ptr<Animation> animation;
	UINT currentKeyFrameIdx = 0;

	float sumTime = 0.0f;

public:
	Animator();
	~Animator();

	void Init() override;
	void Update() override;

	void SetAnimation(const std::shared_ptr<Animation> animation);
	const std::shared_ptr<Animation> GetAnimation() const;
	const KeyFrame& GetCurrentKeyFrame() const;
};

