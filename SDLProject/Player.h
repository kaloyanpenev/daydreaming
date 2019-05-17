#pragma once
#include "AnimationSprite.h"
//#include "Cursor.h"

class Player :
	public AnimationSprite
{
private:
	int moveSpeed;
	bool shooting;
	float movetimer;
public:
	Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalAnimRows, int _totalAnimColumns, float _animSpeed, int _moveSpeed);
	~Player();
	void HandleShooting();
	void ShootAnimation();
	void Move();
	void Draw(float angle);
};

