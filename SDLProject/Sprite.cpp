#include "Sprite.h"



Sprite::Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h)
{
	SDL_Surface* imageFile = IMG_Load(_file);
	image = SDL_CreateTextureFromSurface(_renderer, imageFile);
	SDL_FreeSurface(imageFile);

	position.x = _x;
	position.y = _y;
	position.w = _w;
	position.h = _h;

	window = _window;
	renderer = _renderer;

}


Sprite::~Sprite()
{
	if (image)
		SDL_DestroyTexture(image);
}

void Sprite::Draw()
{
	if (image)
		SDL_RenderCopy(renderer, image, NULL, &position);
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
