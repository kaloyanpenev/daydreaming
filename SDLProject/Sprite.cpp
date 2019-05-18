#include "Sprite.h"



Sprite::Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h)
{
	SDL_Surface* imageFile = IMG_Load(_file);
	texture = SDL_CreateTextureFromSurface(_renderer, imageFile);
	SDL_FreeSurface(imageFile);

	position.x = _x;
	position.y = _y;
	position.w = _w;
	position.h = _h;

	window = _window;
	renderer = _renderer;
	active = true;
}


Sprite::~Sprite()
{
	if (texture)
		SDL_DestroyTexture(texture);
}
bool Sprite::getActive()
{
	return active;
}
void Sprite::setActive(bool _state)
{
	active = _state;
}

void Sprite::Draw()
{
	if (texture)
		SDL_RenderCopy(renderer, texture, NULL, &position);
}
void Sprite::setPosition(Vector2 _pos)
{
	position.x = int(_pos.x);
	position.y = int(_pos.y);
}
void Sprite::setPosition(int _x, int _y)
{
	position.x = _x;
	position.y = _y;
}
Vector2 Sprite::getPosition()
{
	return { float(position.x), float(position.y) };
}
void Sprite::setObject(SDL_Rect _rect)
{
	position.x = _rect.x;
	position.y = _rect.y;
	position.w = _rect.w;
	position.h = _rect.h;
}
SDL_Rect Sprite::getObject()
{
	return position;
}
void Sprite::BoundToScreen(int _screenWidth, int _screenHeight)
{
	if (position.x <= 0)
	{
		position.x = 0;
	}
	if (position.y <= 0)
	{
		position.y = 0;
	}
	if (position.x >= _screenWidth - position.w)
	{
		position.x = _screenWidth - position.w;
	}
	if (position.y >= _screenHeight - position.h)
	{
		position.y = _screenHeight - position.h;
	}
}