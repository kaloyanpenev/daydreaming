#include "Bullet.h"



Bullet::Bullet(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
}


Bullet::~Bullet()
{
}

void Bullet::Fire(int _atX, int _atY, int _dirX, int _dirY, float _angle)
{
	position.x = _atX;
	position.y = _atY;
	start = { float(_atX), float(_atY) };
	direction = { float(_dirX), float(_dirY) };
	active = true;
	angle = _angle;
}
void Bullet::Update(float _speed)
{
	Vector2 temp{ direction.x - start.x, direction.y - start.y };
	temp = temp.normalized();
	position.x += int(temp.x * _speed);
	position.y += int(temp.y * _speed);
}
void Bullet::Draw()
{
	if (texture)
	{
		SDL_RenderCopyEx(renderer, texture, NULL, &position, angle, NULL, SDL_FLIP_NONE);
	}
}
bool Bullet::inScreen(int _screenWidth, int _screenHeight)
{
	if (position.x < 0)
		return false;
	if (position.y < 0)
		return false;
	if (position.x > _screenWidth - position.w)
		return false;
	if (position.y > _screenHeight - position.h)
		return false;
	return true;

}