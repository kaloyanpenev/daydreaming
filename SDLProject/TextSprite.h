#ifndef _TEXTSPRITE_H_
#define _TEXTSPRITE_H_

#include <SDL_ttf.h>
#include "Sprite.h"


class TextSprite :
	public Sprite
{
private:
	TTF_Font* m_font = nullptr;
	SDL_Color m_color;
public:
	TextSprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _fontFile, int _x, int _y, int _w, int _h, int _fontSize, std::string _message, SDL_Color _color);
	~TextSprite();
	void UpdateFont(std::string _message);
	void CloseFont();
};

#endif 