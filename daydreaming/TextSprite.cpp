#include "TextSprite.h"


TextSprite::TextSprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _fontFile, int _x, int _y, int _w, int _h, int _fontSize, std::string _message, SDL_Color _color)
	: Sprite(_window, _renderer, _x, _y, _w, _h), m_font(TTF_OpenFont(_fontFile, _fontSize))
{
	m_color = _color;
}
TextSprite::~TextSprite()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	TTF_CloseFont(m_font);
	m_font = nullptr;
}
void TextSprite::Update(std::string _message)
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, _message.c_str(), m_color);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			position.w = textSurface->w;
			position.h = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
}
void TextSprite::CloseFont()
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
}