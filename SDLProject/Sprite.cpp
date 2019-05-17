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
	dead = false;
}


Sprite::~Sprite()
{
	if (texture)
		SDL_DestroyTexture(texture);
}
bool Sprite::GetLiveState()
{
	return dead;
}

void Sprite::Draw()
{
	if (texture)
		SDL_RenderCopy(renderer, texture, NULL, &position);
}
SDL_Rect Sprite::getPosition()
{
	return position;
}
void Sprite::setPosition(SDL_Rect _rect)
{
	position.x = _rect.x;
	position.y = _rect.y;
	position.w = _rect.w;
	position.h = _rect.h;
}
void Sprite::BoundToScreen()
{
	if (position.x <= 0)
	{
		position.x = 0;
	}
	if (position.y <= 0)
	{
		position.y = 0;
	}
	if (position.x >= 1280 - position.w)
	{
		position.x = 1280 - position.w;
	}
	if (position.y >= 720 - position.h)
	{
		position.y = 720 - position.h;
	}
}