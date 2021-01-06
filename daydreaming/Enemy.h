#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "Sprite.h"
#include <ctime>


class Enemy :
	public Sprite
{
protected:
	std::vector<Vector2> m_waypoints;
	unsigned int m_currentWaypoint;
	Vector2 m_currentPosition;
	Vector2 m_resultPos;
	//static int m_enemyCount;
	//static int m_gamemode;
	//static int m_difficulty;
	

	//int m_survivalTimer;
	int m_updateTimer;
	float m_step;
public:
	Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Enemy();
	void Update(int _smoothness, float _enemySpeed, int _screenWidth, int _screenHeight);
	void InitializeWaypoints(int waypointCount, int _screenWidth, int _screenHeight);
};

#endif