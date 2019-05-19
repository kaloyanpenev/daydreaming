#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <iostream>
#include <vector>
#include <ctime>
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





class GameLoop
{
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	const float FRAME_RATE = 60.0f;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* screenSurface = nullptr;

	//sounds
	Mix_Music* musicTrack = nullptr;
	Mix_Chunk* deathSFX = nullptr;

public:
	GameLoop(int _screenWidth, int _screenHeight);
	~GameLoop();
	bool InitializeSDL();
	void CloseSDL();
	void RunGame();
};

#endif