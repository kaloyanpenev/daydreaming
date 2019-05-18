#include "Player.h"



Player::Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalAnimRows, int _totalAnimColumns, int _animSpeed, int _moveSpeed) :
	AnimationSprite(_window, _renderer, _file, _x, _y, _w, _h, _totalAnimRows, _totalAnimColumns, _animSpeed, true), moveSpeed(_moveSpeed)
{
	shooting = false;
	reloadTimer = 0;
	playShootAnimation = false;
}


Player::~Player()
{
}
void Player::Move(int _screenWidth, int _screenHeight)
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
		BoundToScreen(_screenWidth, _screenHeight);
		
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

void Player::HandleShooting(int _reloadTime)
{
	reloadTimer++;
	if (reloadTimer > _reloadTime)
	{
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) //if left button is clicked and reloadtimer == reloadtime
		{
			shooting = true;
			reloadTimer = 0;
		}
	}
	else
		return;
}

void Player::ShootAnimation()
{
	if (shooting)
		playShootAnimation = true;
	if (playShootAnimation)
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
				currentColumn = 0; // go back to col 0
				playShootAnimation = false;
			}
		}
	}
}
void Player::setShooting(bool _state)
{
	shooting = _state;
}
bool Player::getShooting()
{
	return shooting;
}