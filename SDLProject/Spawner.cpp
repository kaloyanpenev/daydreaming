#include "Spawner.h"



Spawner::Spawner(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, int _speed, bool _loop) :
	AnimationSprite(_window, _renderer, _file, _x, _y, _w, _h, _totalRows, _totalColumns, _speed, _loop)
{
	spawnTimer = 0;
	spawning = true;
	needSpawner = false;
}


Spawner::~Spawner()
{

}
void Spawner::setSpawners(int _spawnerCount)
{
	if (spawning)
	{
		for (int i = 0; i < _spawnerCount; i++)
		{
			spawnPoints.push_back({ float(rand() % 1216), float(rand() % 656) }); //set spawnpoints at random, 1216= 1280-64 and 656 = 720-64 
			std::cout << "\nCurrent SpawnPoint: " << spawnPoints.at(i).x << "," << spawnPoints.at(i).y;
		}
		spawning = false;
	}
	else
		return;
}
void Spawner::setCurrentSpawnPoint(int _spawnPoint)
{
	currentSpawnPoint = _spawnPoint;
}
Vector2 Spawner::getSpawnPoint(int _at)
{
	return spawnPoints.at(_at);
}
int Spawner::getVectorSize()
{
	return spawnPoints.size();
}