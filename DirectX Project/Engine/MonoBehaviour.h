#pragma once

#include "Component.h"

class MonoBehaviour : public Component
{
private:
	using Base = Component;

public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

	virtual void Init()			override;
	virtual void BeginPlay()	override;
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render()		override;
	virtual void PostRender()	override;
	virtual void Release()		override;
};

