#pragma once
#include "AnimationSprite.h"
#include <ctime>


class Enemy :
	public Sprite
{
protected:
	std::vector<Vector2> waypoints;
	unsigned int currentWaypoint;
	Vector2 currentPosition;
	Vector2 resultPos;


	int updateTimer;
	float step;
public:
	Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Enemy();
	void Update(int _smoothness, float _enemySpeed, int _screenWidth, int _screenHeight);
	void InitializeWaypoints(int waypointCount, int _screenWidth, int _screenHeight);
	//void SpawnEnemies(int _timer, int _enemyCount, std::vector<Enemy*>& enemies);
};

