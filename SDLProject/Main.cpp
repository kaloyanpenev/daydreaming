#include "GameManager.h"



int main( int argc, char* args[] )
{
	GameLoop* game = new GameLoop(1280,720);
	game->RunGame();
	delete game;
	return 0;
}