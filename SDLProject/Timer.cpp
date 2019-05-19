#include "Timer.h"

Timer::Timer()
{
}
Timer::~Timer()
{
}
void Timer::reset()
{
	m_startTicks = SDL_GetTicks();
	m_elapsedTicks = 0;
	m_deltaTime = 0;
}
void Timer::Update()
{
	m_elapsedTicks = SDL_GetTicks() - m_startTicks;
	m_deltaTime = m_elapsedTicks * 0.001f;
}
