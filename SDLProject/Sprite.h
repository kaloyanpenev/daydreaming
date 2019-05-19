#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Vector2.h"


///Some parts of this function were written by following this guide:-
///Pete Allen, 2019, Bournemouth University, Sprites Guide 
class Sprite
{
protected:
	//note: variable names are unchanged from the citation source(as a result in format of "position" instead of "m_position", this was to respect the guidelines of citations!)
	SDL_Window* window = nullptr;;
	SDL_Renderer* renderer = nullptr;;
	SDL_Texture* texture = nullptr;
	SDL_Rect position;
	bool m_active;
public:
	Sprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h); //graphics sprites
	Sprite(SDL_Window* _window, SDL_Renderer* _renderer, int _x, int _y, int _w, int _h); 
	~Sprite();
	bool getActive() const { return m_active; }
	void setActive(bool _state) { m_active = _state; }
	void setObject(SDL_Rect);
	SDL_Rect getObject() const { return position; }
	void setPosition(int _x, int _y);
	void setPosition(Vector2 _pos);
	Vector2 getPosition() const { return { float(position.x), float(position.y) }; }
	virtual void Draw();
	void BoundToScreen(int _screenWidth, int _screenHeight);
};

#endif