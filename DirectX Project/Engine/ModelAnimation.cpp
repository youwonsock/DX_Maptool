 #include "pch.h"
#include "ModelAnimation.h"

std::shared_ptr<ModelKeyframe> ModelAnimation::GetKeyframe(std::wstring boneName)
{
	auto findit = keyframes.find(boneName);
	
	if (findit != keyframes.end())
		return findit->second;

	return nullptr;
}
