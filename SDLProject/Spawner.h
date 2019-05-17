#pragma once
#include "AnimationSprite.h"
class Spawner :
	public AnimationSprite
{
private:
	std::vector<Vector2> spawnPoints;
	int currentSpawnPoint;
	int spawnTimer;
	bool spawning;
	bool needSpawner;
public:
	Spawner(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h, int _totalRows, int _totalColumns, float _speed, bool _loop);
	~Spawner();
	void setSpawners(int _spawnerCount);
	void setCurrentSpawnPoint(int _spawnPoint);
	Vector2 getSpawnPoint(int _at);
	int getVectorSize();
};

