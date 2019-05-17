#pragma once

#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include "Sprite.h"
#include "Player.h"
#include "Cursor.h"
#include "AnimationSprite.h"
#include "Enemy.h"
#include "Timer.h"
#include <ctime>
#include <vector>





class GameLoop
{
private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const int FRAME_RATE = 60;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* screenSurface = nullptr;
public:
	GameLoop();
	~GameLoop();
	bool InitializeSDL();
	void CloseSDL();
	void RunGame();
};
