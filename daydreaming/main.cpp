#include "GameManager.h"



int main( int argc, char* args[] )
{
	GameManager* game = new GameManager(1920, 1080);
	game->RunGame();
	delete game;
	return 0;
}