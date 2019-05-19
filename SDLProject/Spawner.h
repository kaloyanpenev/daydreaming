#ifndef _SPAWNER_H_
#define _SPAWNER_H_
#include "AnimationSprite.h"
class Spawner :
	public AnimationSprite
{
private:
	std::vector<Vector2> m_spawnPoints;
	int m_currentSpawnPoint;
	int m_spawnTimer;
	bool m_spawning;
	bool m_needSpawner;
public:
	Spawner(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, int _speed, bool _loop);
	~Spawner();
	void setSpawners(int _spawnerCount);

	void setCurrentSpawnPoint(int _spawnPoint)	{ m_currentSpawnPoint = _spawnPoint; }
	Vector2 getSpawnPoint(int _at) const { return m_spawnPoints.at(_at); }
	int getVectorSize() const { return m_spawnPoints.size(); }
};

#endif