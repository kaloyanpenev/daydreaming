#pragma once
#include "Sprite.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

class Cursor : 
	public Sprite
{
private:
	bool usingLastPos = false;
	SDL_Point lastPos;
public:
	Cursor(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Cursor();
	void Draw();
	void MouseLook(SDL_Event* e);
	float getAngle(int x1, int y1, int x2, int y2);
	float getAngleBetweenMouseAndRect(SDL_Rect _rect);
	void DrawLineMouseToSprite(Sprite* sprite);
};

