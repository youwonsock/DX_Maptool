#include "pch.h"

#include "Animation.h"
#include "Texture.h"

Animation::Animation() : Base(ResourceType::Animation)
{
}

Animation::~Animation()
{
}

void Animation::Save(const std::wstring& path)
{
	tinyxml2::XMLDocument doc;

	XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	std::string nameStr(GetName().begin(), GetName().end());
	root->SetAttribute("Name", nameStr.c_str());
	root->SetAttribute("Loop", isLooping);
	root->SetAttribute("TexturePath", "TODO");

	for (const auto& keyframe : keyFrames)
	{
		XMLElement* node = doc.NewElement("Keyframe");
		root->LinkEndChild(node);

		node->SetAttribute("OffsetX", keyframe.offset.x);
		node->SetAttribute("OffsetY", keyframe.offset.y);
		node->SetAttribute("SizeX", keyframe.size.x);
		node->SetAttribute("SizeY", keyframe.size.y);
		node->SetAttribute("Time", keyframe.time);
	}

	std::string pathStr(path.begin(), path.end());
	auto result = doc.SaveFile(pathStr.c_str());
	assert(result == XMLError::XML_SUCCESS);
}

void Animation::Load(const std::wstring& path)
{
	tinyxml2::XMLDocument doc;

	std::string pathStr(path.begin(), path.end());
	XMLError error = doc.LoadFile(pathStr.c_str());
	assert(error == XMLError::XML_SUCCESS);

	XmlElement* root = doc.FirstChildElement();
	std::string nameStr = root->Attribute("Name");
	name = std::wstring(nameStr.begin(), nameStr.end());

	isLooping = root->BoolAttribute("Loop");
	this->path = path;

	// Load Texture

	XmlElement* node = root->FirstChildElement();
	for (; node != nullptr; node = node->NextSiblingElement())
	{
		KeyFrame keyframe;

		keyframe.offset.x = node->FloatAttribute("OffsetX");
		keyframe.offset.y = node->FloatAttribute("OffsetY");
		keyframe.size.x = node->FloatAttribute("SizeX");
		keyframe.size.y = node->FloatAttribute("SizeY");
		keyframe.time = node->FloatAttribute("Time");

		AddKeyFrame(keyframe);
	}
}

Vector2& Animation::GetTextureSize()
{
	return texture->GetSize();
}

const KeyFrame& Animation::GetKeyFrame(UINT index) const
{
	if (index >= keyFrames.size())
		assert(false);

	return keyFrames[index];
}


void Animation::AddKeyFrame(const KeyFrame& keyFrame)
{
	keyFrames.push_back(keyFrame);
}
