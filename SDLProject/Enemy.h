#pragma once
#include "AnimationSprite.h"
#include <ctime>


class Enemy :
	public Sprite
{
protected:
	std::vector<Vector2> waypoints;
	int currentWaypoint;
	Vector2 currentPosition;
	Vector2 resultPos;


	int updateTimer;
	float step;
public:
	Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Enemy();
	void Update(int _smoothness, float _enemySpeed);
	void InitializeWaypoints(int waypointCount);
	//void SpawnEnemies(int _timer, int _enemyCount, std::vector<Enemy*>& enemies);
};

