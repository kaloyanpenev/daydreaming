#include "TextSprite.h"


TextSprite::TextSprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _fontFile, int _x, int _y, int _w, int _h, int _fontSize, std::string _message, SDL_Color _color)
	: Sprite(_window, _renderer, _x, _y, _w, _h), m_font(TTF_OpenFont(_fontFile, _fontSize))
{
	m_color = _color;
	SDL_Surface* fontSurface = TTF_RenderText_Solid(m_font, _message.c_str(), _color);
	position.w = fontSurface->w;
	position.h = fontSurface->h;
	texture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
}
TextSprite::~TextSprite()
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
}
void TextSprite::UpdateFont(std::string _message)
{
	SDL_Surface* fontSurface = TTF_RenderText_Solid(m_font, _message.c_str(), m_color);
	position.w = fontSurface->w;
	position.h = fontSurface->h;
	texture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
}
void TextSprite::CloseFont()
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
}