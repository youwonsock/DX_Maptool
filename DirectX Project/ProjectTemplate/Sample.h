#pragma once

// ExLib.h and StdHeader.h must be included in front of Engine.h.
#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

class Sample : public Engine
{
	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Release() override;
};
