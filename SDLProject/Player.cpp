#include "Player.h"



Player::Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalAnimRows, int _totalAnimColumns, float _animSpeed, int _moveSpeed) :
	AnimationSprite(_window, _renderer, _file, _x, _y, _w, _h, _totalAnimRows, _totalAnimColumns, _animSpeed, true), moveSpeed(_moveSpeed)
{
	shooting = false;
}


Player::~Player()
{
}
void Player::Move()
{
	movetimer++;
	if (movetimer == moveSpeed)
	{
		const Uint8* key = SDL_GetKeyboardState(NULL);
		if (key[SDL_SCANCODE_A]) //moving left
		{
			position.x -= 5;
		}
		if (key[SDL_SCANCODE_D])
		{
			position.x += 5;
		}
		if (key[SDL_SCANCODE_W])
		{
			position.y -= 5;
		}
		if (key[SDL_SCANCODE_S])
		{
			position.y += 5;
		}
		BoundToScreen();
		
		movetimer = 0;
	}
}
void Player::Draw(float angle)
{
	if (texture)
	{
		animRect.x = currentColumn * animRect.w;
		animRect.y = currentRow * animRect.h;
		SDL_RenderCopyEx(renderer, texture, &animRect, &position, angle, NULL, SDL_FLIP_NONE);
	}
}

void Player::HandleShooting()
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) //if left button is clicked
	{
		shooting = true;
	}
}

void Player::ShootAnimation()
{
	if (shooting)
	{
		timer++;
		if (timer == speed)
		{
			timer = 0;
			//if time to proceed to next animation sprite is right
			currentColumn++; //go to next column
				//if reached max columns
			if (currentColumn == totalColumns)
			{
				currentColumn = 0; // go back to col 0
				shooting = false;
			}
		}
	}
}