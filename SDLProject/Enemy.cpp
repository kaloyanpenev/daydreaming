#include "Enemy.h"



Enemy::Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	step = 0.0f;
	srand(unsigned(time(NULL)));
	timer = 0;
	for (int i = 0; i < 4; i++)
	{
		waypoints.push_back({ float(rand() % 1280), float(rand() % 720) });
	}
}


Enemy::~Enemy()
{
}


void Enemy::Update(float _speed)
{
	Vector2 positionVec{ float(position.x), float(position.y) };
	timer++;
	if (positionVec != waypoints.at(0) && timer >= _speed)
	{
		if (step < 1)
		{
			Vector2 resultPos = lerp(positionVec, waypoints.at(0), step);
			position.x = resultPos.x;
			position.y = resultPos.y;
			step += 0.017; //deltatime
			timer = 0;
		}
	}
	BoundToScreen();
}