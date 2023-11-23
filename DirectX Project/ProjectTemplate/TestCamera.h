#pragma once

#include "Engine/ExLib.h"	  
#include "Engine/StdHeader.h" 
#include "Engine/Engine.h"

#include "Engine/MonoBehaviour.h"

class TestCamera : public MonoBehaviour
{
private:
	float speed = 10.0f;
	
public:
	virtual void BeginPlay()	override;
	virtual void Update()		override;
};

