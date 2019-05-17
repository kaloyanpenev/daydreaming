#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

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
	inline bool GetLiveState();

	SDL_Rect getPosition();
	void setPosition(SDL_Rect);
	virtual void Draw();
	void BoundToScreen();
};

