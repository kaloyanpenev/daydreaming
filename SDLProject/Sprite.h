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
	bool active;
public:
	Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Sprite();
	bool getActive();
	void setActive(bool _state);
	void setObject(SDL_Rect);
	SDL_Rect getObject();
	void setPosition(int _x, int _y);
	void setPosition(Vector2 _pos);
	Vector2 getPosition();
	virtual void Draw();
	void BoundToScreen(int _screenWidth, int _screenHeight);
};

