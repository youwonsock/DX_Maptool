#pragma once

class IExecute
{
public:
	virtual void Init() abstract;

	virtual void FixedUpdate() abstract;
	virtual void Update() abstract;
	virtual void PostUpdate() abstract;

	virtual void PreRender() abstract;
	virtual void Render() abstract;
	virtual void PostRender() abstract;

	virtual void Release() abstract;
};

