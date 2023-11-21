#pragma once

#include "Singleton.hpp"
#include "Struct.h"

class ShaderManager : public Singleton<ShaderManager>
{
private:
	friend class Singleton<ShaderManager>;

	std::unordered_map<std::wstring, ShaderDesc> shaders;
public:
	ShaderDesc GetEffect(std::wstring fileName);
};