#include "Enemy.h"



Enemy::Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	currentWaypoint = 0;
	step = 0.0f;
	srand(unsigned(time(NULL)));
	updateTimer = 0;
	//InitializeWaypoints(3);
}


Enemy::~Enemy()
{
}


void Enemy::Update(int _smoothness, float _enemySpeed)
{
	Vector2 positionVec{ float(position.x), float(position.y) };

	updateTimer++;
	if (updateTimer >= _smoothness)
	{
		Vector2 distance{ waypoints.at(currentWaypoint) - positionVec };
		if (distance.magnitude() > position.w) //if the distance to the waypoint is still bigger than our object's width(as a placeholder value for size(64x64), as to not get stuck at the extremities of the window)
		{
			Vector2 resultPos = lerp(positionVec, waypoints.at(currentWaypoint), step); //lerp towards it
			position.x = resultPos.x; //assign resultpos to position rect
			position.y = resultPos.y;
			step += _enemySpeed;
			updateTimer = 0;
		}
		else if (currentWaypoint < waypoints.size())
		{
			step = 0;
			updateTimer = 0;
			waypoints.push_back({ float(rand() % 1280), float(rand() % 720) }); //pushback a new waypoint
			waypoints.erase(waypoints.begin()); //erase waypoint at beginning of vector
			//this way our vector doesn't grow, however we can access the next 3 waypoints(for more complicated enemy AI in future iterations)
		}
	}
	BoundToScreen();
}

void Enemy::InitializeWaypoints(int waypointCount)
{
	for(int i = 0; i < waypointCount; i++)
		waypoints.push_back({ float(rand() % 1280), float(rand() % 720) });
}

//void Enemy::SpawnEnemies(int _timer, int _enemyCount, std::vector<Enemy*>& enemies)
//{
//	//instantiate a temporary enemy object
//	Enemy* temp = new Enemy(window, renderer, "assets/littleman.bmp", int(spawnPoints.at(currentSpawnPoint).x), int(spawnPoints.at(currentSpawnPoint).y), 64, 64);
//	temp->InitializeWaypoints(3);	//intialize the waypoints that the enemy object is going to follow
//	enemies.push_back(temp);	//add the temporary enemy object to the enemy vector
//	delete temp;		//delete temporary object
//}