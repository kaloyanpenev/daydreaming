#include "GameManager.h"



int main( int argc, char* args[] )
{
	GameManager* game = new GameManager(1280,720);
	game->RunGame();
	delete game;
	return 0;
}