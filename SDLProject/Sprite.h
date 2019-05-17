#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Vector2.h"

class Sprite
{
protected:
	SDL_Window* window = nullptr;;
	SDL_Renderer* renderer = nullptr;;
	SDL_Texture* texture = nullptr;
	SDL_Rect position;
	bool dead;
public:
	Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Sprite();
	bool GetDead();
	void SetDead(bool _dead);
	SDL_Rect getPosition();
	void setObject(SDL_Rect);
	void setPosition(int _x, int _y);
	virtual void Draw();
	void BoundToScreen();
};

