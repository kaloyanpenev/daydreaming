#include "Player.h"



Player::Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _moveSpeed) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h), moveSpeed(_moveSpeed)
{
}


Player::~Player()
{
}
void Player::Move()
{
	int maxMoveSpeed = 10;
	const Uint8* key = SDL_GetKeyboardState(NULL);
	if (key[SDL_SCANCODE_A])
	{
		position.x -= 1;
	}
	if (key[SDL_SCANCODE_D])
	{
		position.x += 1;
	}
	if (key[SDL_SCANCODE_W])
		position.y -= 1;
	if (key[SDL_SCANCODE_S])
		position.y += 1;
	SDL_Delay(maxMoveSpeed - moveSpeed);
}
void Player::Draw(float angle)
{
	if (image)
		SDL_RenderCopyEx(renderer, image, NULL, &position, angle, NULL, SDL_FLIP_NONE);
}