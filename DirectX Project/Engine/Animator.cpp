#include "pch.h"
#include "Animator.h"

#include "Animation.h"
#include "TimeManager.h"

Animator::Animator() : Base(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{
}

void Animator::Update()
{
	std::shared_ptr<Animation> animation = GetAnimation();
	if(animation == nullptr)
		return;

	const KeyFrame& keyframe = animation->GetKeyFrame(currentKeyFrameIdx);

	float deltaTime = TimeManager::GetInstance().GetDeltaTime();
	sumTime += deltaTime;

	if (sumTime >= keyframe.time)
	{
		currentKeyFrameIdx++;
		UINT totalCount = animation->GetKeyFrameCount();

		if (currentKeyFrameIdx >= totalCount)
		{
			if (animation->GetIsLooping())
				currentKeyFrameIdx = 0;
			else
				currentKeyFrameIdx = totalCount - 1;
		}

		sumTime = 0.f;
	}
}

void Animator::SetAnimation(const std::shared_ptr<Animation> animation)
{
	this->animation = animation;
}

const std::shared_ptr<Animation> Animator::GetAnimation() const
{
	return animation;
}

const KeyFrame& Animator::GetCurrentKeyFrame() const
{
	return animation->GetKeyFrame(currentKeyFrameIdx);
}