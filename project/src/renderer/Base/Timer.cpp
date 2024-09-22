#include "Timer.h"
#include <GLFW/glfw3.h>

float Timer::deltaTime = 0.f;
float Timer::lastFrameTime = 0.f;
Timer::Timer()
{
}

Timer::~Timer()
{
}

float Timer::GetTime()
{
	return glfwGetTime();
}

float Timer::GetDeltaTime()
{
	return deltaTime;
}

void Timer::Tick()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;
}
