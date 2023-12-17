#pragma once

#include "Component.h"
#include "Types.h"

class Light : public Component
{
private:
	LightDesc lightDesc;

public:
	Light();
	virtual ~Light();

	virtual void Update();

	LightDesc& GetLightDesc() { return lightDesc; }
	void SetLightDesc(const LightDesc& desc) { lightDesc = desc; }
	void SetAmbient(const Color& color) { lightDesc.ambient = color; }
	void SetDiffuse(const Color& color) { lightDesc.diffuse = color; }
	void SetSpecular(const Color& color) { lightDesc.specular = color; }
	void SetEmissive(const Color& color) { lightDesc.emissive = color; }
	void SetDirection(const Vector3& dir) { lightDesc.direction = dir; }

};

