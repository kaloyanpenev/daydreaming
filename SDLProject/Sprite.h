#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Sprite
{
protected:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* image;
	SDL_Rect position;
public:
	Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Sprite();
	virtual void Draw();
	SDL_Rect getPosition();
	void setPosition(SDL_Rect);
};

