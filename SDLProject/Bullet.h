#pragma once
#include "Sprite.h"
class Bullet :
	public Sprite
{
private:
	Vector2 direction;
	Vector2 start;
	float angle;
public:
	Bullet(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Bullet();
	void Fire(int _atX, int _atY, int _dirX, int _dirY, float _angle);
	void Update(float _speed);
	void Draw() override;
	bool inScreen(int _screenWidth, int _screenHeight);
};

