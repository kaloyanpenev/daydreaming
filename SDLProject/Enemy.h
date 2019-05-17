#pragma once
#include "AnimationSprite.h"
#include "Vector2.h"
#include <vector>
#include <ctime>


class Enemy :
	public Sprite
{
protected:
	std::vector<Vector2> waypoints;
	Vector2 currentWaypoint;
	Vector2 currentPosition;
	Vector2 resultPos;
	int timer;
	float step;
	float deltaTime;
public:
	Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Enemy();
	void Update(float _speed);
};

