#include "Player.h"



Player::Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalAnimRows, int _totalAnimColumns, float _animSpeed, int _moveSpeed) :
	AnimationSprite(_window, _renderer, _file, _x, _y, _w, _h, _totalAnimRows, _totalAnimColumns, _animSpeed, true), m_moveSpeed(_moveSpeed)
{
	m_shooting = false;
	m_reloadTimer = 0;
	m_playShootAnimation = false;
}


Player::~Player()
{
}
void Player::Move(int _screenWidth, int _screenHeight)
{
	m_movetimer++;
	if (m_movetimer == m_moveSpeed)
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
		
		m_movetimer = 0;
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

void Player::HandleShooting(float _reloadTime)
{
	m_reloadTimer++;
	if (m_reloadTimer >= _reloadTime)
	{
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) //if left button is clicked and reloadtimer == reloadtime
		{
			m_shooting = true;
			m_reloadTimer = 0;
		}
	}
}

void Player::ShootAnimation()
{
	if (m_shooting)
	{
		m_playShootAnimation = true;
	}
	if (m_playShootAnimation)
	{
		timer++;
		//if time to proceed to next animation sprite is right
		if (timer == m_speed)
		{
			timer = 0;
			currentColumn++; //go to next column
				//if reached max columns
			if (currentColumn == totalColumns)
			{
				currentColumn = 0; // go back to col 0
				m_playShootAnimation = false;
			}
		}
	}
}