#ifndef _CURSOR_H_
#define _CURSOR_H_
#include "Sprite.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

class Cursor : 
	public Sprite
{
private:
	bool m_usingLastPos;
	SDL_Point m_lastPos;
	int m_MouseX, m_MouseY;
public:
	Cursor(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h);
	~Cursor();
	void Draw() override;
	void MouseLook(SDL_Event* e);
	float AngleBetweenMouseAndRect(SDL_Rect _rect);
	void DrawLineMouseToSprite(Sprite* _sprite);
};

#endif