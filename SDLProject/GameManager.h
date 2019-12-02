#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Sprite.h"
#include "Player.h"
#include "Bullet.h"
#include "Cursor.h"
#include "AnimationSprite.h"
#include "Enemy.h"
#include "TextSprite.h"
#include "Timer.h"
#include "Spawner.h"
#include "Vector2.h"





class GameManager
{
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	const float FRAME_RATE = 60.0f;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* screenSurface = nullptr;
	bool m_restartFlag;
	bool m_gameExitFlag;
	bool m_winFlag;
	int m_score;
	int m_enemyCount = 15;

	//sounds
	Mix_Music* m_musicTrack = nullptr;
	Mix_Chunk* m_enemySFX = nullptr;
	Mix_Chunk* m_playerDeathSFX = nullptr;
	Mix_Chunk* m_shootSFX = nullptr;

public:
	static int m_gamemode;
	GameManager(int _screenWidth, int _screenHeight);
	~GameManager();
	bool InitializeSDL();
	void CloseSDL();
	void RunGame();
	void LoadMusic();
	void CloseMusic();
	bool Menu();
	void RestartScreen();
	void GameLoop();
	bool MouseOverButton(SDL_Rect _button, const int _mouseX, const int _mouseY);
	void ToggleMusic(SDL_Event* _e);
	/* gamemode:
	0 - survival
	1 - time-trial
	2 - training    */
	void setGamemode(int _gamemode) { m_gamemode = _gamemode; }
	int getGamemode() { return m_gamemode; }
};

#endif