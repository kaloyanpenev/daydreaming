#include "Enemy.h"



Enemy::Enemy(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	m_currentWaypoint = 0;
	m_step = 0.0f;
	m_updateTimer = 0;
	//InitializeWaypoints(3);
}


Enemy::~Enemy()
{
}


void Enemy::Update(int _smoothness, float _enemySpeed, int _screenWidth, int _screenHeight)
{
	Vector2 positionVec{ float(position.x), float(position.y) };

	m_updateTimer++;
	if (m_updateTimer >= _smoothness)
	{
		Vector2 distance{ m_waypoints.at(m_currentWaypoint) - positionVec };
		if (distance.magnitude() > position.w) //if the distance to the waypoint is still bigger than our object's width(as a placeholder value for size(64x64), as to not get stuck at the extremities of the window)
		{
			Vector2 resultPos = lerp(positionVec, m_waypoints.at(m_currentWaypoint), m_step); //lerp towards it
			position.x = int(resultPos.x); //assign resultpos to position rect
			position.y = int(resultPos.y);
			m_step += _enemySpeed;
			m_updateTimer = 0;
		}
		else if (m_currentWaypoint < m_waypoints.size())
		{
			m_step = 0;
			m_updateTimer = 0;
			m_waypoints.push_back({ float(rand() % 1280), float(rand() % 720) }); //pushback a new waypoint
			m_waypoints.erase(m_waypoints.begin()); //erase waypoint at beginning of vector
			//this way our vector doesn't grow, however we can access the next 3 waypoints(for more complicated enemy AI in future iterations)
		}
	}
	BoundToScreen(_screenWidth, _screenHeight);
}

void Enemy::InitializeWaypoints(int waypointCount, int _screenWidth, int _screenHeight)
{
	for (int i = 0; i < waypointCount; i++)
	{
		m_waypoints.push_back({ float(rand() % _screenWidth), float(rand() % _screenHeight) });
	}
}