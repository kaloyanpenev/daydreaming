#include "AnimationSprite.h"



AnimationSprite::AnimationSprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, float _speed, bool _loop) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	currentRow = 0;
	currentColumn = 0;

	totalRows = _totalRows;
	totalColumns = _totalColumns;
	animRect.x = 0;
	animRect.y = 0;
	animRect.w = _w;
	animRect.h = _h;

	speed = _speed;
	loop = _loop;
	timer = 0;
	canRestart = false;
}


AnimationSprite::~AnimationSprite()
{
}


void AnimationSprite::Animate()
{
	timer++;
	//if time to proceed to next animation sprite is right
	if (timer == speed)
	{
		timer = 0;
		currentColumn++; //go to next column
		//if reached max columns
		if (currentColumn == totalColumns)
		{
			currentColumn = 0; // go back to co.l 0
			currentRow++; // go to next sprite sheet
			//if reached max rows
			if (currentRow == totalRows)
			{
				if (loop)
					currentRow = 0;
				else
				{
					dead = true;
					canRestart = true;
				}
			}
		}
	}
	if (canRestart && !dead)
	{
		currentRow = 0;
		currentColumn = 0;
		timer = 0;
		canRestart = false;
	}
}

void AnimationSprite::Draw()
{
	animRect.x = currentColumn * animRect.w;
	animRect.y = currentRow * animRect.h;
	if (texture)
		SDL_RenderCopy(renderer, texture, &animRect, &position);
}