#pragma once

#include "Base/core.h"

class JNAPI Timer
{
private:
	static float deltaTime;
	static float lastFrameTime;

public:
	Timer();
	~Timer();

	static float GetTime();
	static void Tick();
	static float GetDeltaTime();
};