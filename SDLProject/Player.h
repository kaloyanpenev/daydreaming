#pragma once
#include "Sprite.h"
#include "Cursor.h"

class Player :
	public Sprite
{
private:
	int moveSpeed;
public:
	Player(SDL_Window* window, SDL_Renderer* renderer, const char* _file, int _x, int _y, int _w, int _h, int _moveSpeed);
	~Player();
	void Move();
	void Draw(float angle);
};

