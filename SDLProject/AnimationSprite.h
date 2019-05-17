#pragma once
#include "Sprite.h"
class AnimationSprite :
	public Sprite
{
protected:
	SDL_Rect animRect;
	int totalRows;
	int totalColumns;
	int currentRow;
	int currentColumn;
	
	int timer;
	float speed;
	bool loop;
public:
	AnimationSprite(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, float _speed, bool _loop);
	virtual void Animate();
	void Draw() override;
	~AnimationSprite();
};

