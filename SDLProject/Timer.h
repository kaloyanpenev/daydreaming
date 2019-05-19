#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL.h>


///Timer class
///Partly modified, source from:-
///Ather Omar(February 15, 2017). SDL 2.0 Tutorials: 2.Creating a Timer for our game
///Available at: https://www.youtube.com/watch?v=z9U-Jif4RVU
class Timer
{

private:
	unsigned int m_startTicks;
	unsigned int m_elapsedTicks;
	float m_deltaTime;
	float m_timeScale;
public:
	Timer();
	~Timer();
	void reset();
	inline float getDeltaTime() const { return m_deltaTime; }
	void Update();
};

#endif