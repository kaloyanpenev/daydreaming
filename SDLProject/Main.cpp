#include "GameLoop.h"



int main( int argc, char* args[] )
{
	GameLoop* game = new GameLoop();
	game->RunGame();
	delete game;
	return 0;
}