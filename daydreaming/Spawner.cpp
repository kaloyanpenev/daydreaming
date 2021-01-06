#include "Spawner.h"



Spawner::Spawner(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, float _speed, bool _loop) :
	AnimationSprite(_window, _renderer, _file, _x, _y, _w, _h, _totalRows, _totalColumns, _speed, _loop)
{
	m_spawning = true;
}


Spawner::~Spawner()
{

}
void Spawner::setSpawners(int _spawnerCount)
{
	if (m_spawning)
	{
		for (int i = 0; i < _spawnerCount; i++)
		{
			//set spawnpoints at random, 1216= 1280-64-1 and 656 = 720-64-1 so that enemies don't get stuck at the extremities of the window
			m_spawnPoints.push_back({ float(rand() % 1215), float(rand() % 655) });
		}
		m_spawning = false;
	}
}
void Spawner::addSpawnPoint()
{
	m_spawnPoints.push_back({ float(rand() % 1215), float(rand() % 655) });
}