#pragma once

class TestCamera : public MonoBehaviour
{
private:
	float speed = 10.0f;
	
public:
	virtual void BeginPlay()	override;
	virtual void Update()		override;
};

