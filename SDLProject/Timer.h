#pragma once
#include <SDL.h>
class Timer
{
private:
	static Timer* m_instance;
	unsigned int startTicks;
	unsigned int elapsedTicks;
	float deltaTime;
	float timeScale;
public:
	Timer();
	~Timer();
	static Timer* instance();
	static void release();
	void reset();
	float getDeltaTime();
	void setTimeScale(float t);
	float getTimeScale();
	void Update();
};

