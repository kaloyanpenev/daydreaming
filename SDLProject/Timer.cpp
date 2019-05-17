#include "Timer.h"

//Timer* Timer::m_instance = nullptr;
//
//Timer* Timer::instance()
//{
//	if (m_instance = nullptr)
//		m_instance = new Timer();
//
//	return m_instance;
//}

Timer::Timer()
{
}
Timer::~Timer()
{
}
void Timer::reset()
{
	startTicks = SDL_GetTicks();
	elapsedTicks = 0;
	deltaTime = 0;
}

float Timer::getDeltaTime()
{
	return deltaTime;
}
float Timer::getTimeScale()
{
	return timeScale;
}
void Timer::setTimeScale(float _ts)
{
	timeScale = _ts;
}
void Timer::Update()
{
	elapsedTicks = SDL_GetTicks() - startTicks;
	deltaTime = elapsedTicks * 0.001f;
}
