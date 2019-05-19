#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "AnimationSprite.h"
//#include "Cursor.h"

class Player :
	public AnimationSprite
{
private:
	int m_moveSpeed;
	bool m_shooting;
	float m_movetimer;
	int m_reloadTimer;
	bool m_playShootAnimation;
public:
	Player(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalAnimRows, int _totalAnimColumns, int _animSpeed, int _moveSpeed);
	~Player();
	void HandleShooting(int _reloadTime);
	void ShootAnimation();
	void Move(int _screenWidth, int _screenHeight);
	void Draw(float angle);
	bool getShooting() const { return m_shooting; }
	void setShooting(bool _state) { m_shooting = _state; }
};

#endif