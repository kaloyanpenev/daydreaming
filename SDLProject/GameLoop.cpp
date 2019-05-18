#include "GameLoop.h"


GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::RunGame()
{
	if (!InitializeSDL())
		std::cout << "Error loading SDL: " << SDL_GetError() << std::endl;

	int enemyCount = 12;
	int currentSpawnPoint = 0;
	int playerMS = 1;
	int spawningSpeed = 3;

	//using smart pointers to instantiate some of our game objects
	//unique pointer is used when we will have only 1 instance of the object (e.g. background, player, etc)
	//shared pointer is used when we have a transfer of ownership when working with objects
	std::unique_ptr<Timer> time{ new Timer() };
	std::unique_ptr<Sprite> background{ new Sprite(window, renderer, "assets/Background.png", 0, 0, 1280, 720) }; //unique pointer because we have only one background
	std::unique_ptr<Player> player{ new Player(window, renderer, "assets/shootAnim1x4.png", int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 2), 64, 64, 1, 4, 2, playerMS) }; //unique pointer, only 1 player
	std::unique_ptr<Cursor> mouse{ new Cursor(window, renderer, "assets/xh.png", int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 2), 16, 16) }; //unique pointer, only 1 mouse
	std::unique_ptr<Spawner> spawner{ new Spawner(window, renderer, "assets/spawner1x8.png", 0, 0, 64, 64, 1, 8, spawningSpeed, false) }; //we will be recycling our spawner object, but for future iterations we could have a vector of spawners
	std::vector<std::shared_ptr<Enemy>> enemies; //vector of shared pointers because we will be pushing back temporary objects(ownership gets transfered)
	std::vector<std::shared_ptr<AnimationSprite>> explosionList; //vector of shared ptrs
	std::vector<std::shared_ptr<Bullet>> bulletList; //vector of shared ptrs


	SDL_Event e;
	bool exitFlag= false;

	while (!exitFlag)
	{
		time->Update();
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				exitFlag = true;
			}
		}
		if (time->getDeltaTime() >= 1.0f / FRAME_RATE) //limit framerate
		{
			SDL_RenderClear(renderer);
			background->Draw();

			//HANDLE MOUSE
			mouse->MouseLook(&e);
			mouse->DrawLineMouseToSprite(player.get());
			mouse->Draw();

			//HANDLE PLAYER INTERACTION
			player->Move(SCREEN_WIDTH, SCREEN_HEIGHT);
			if (!player->getShooting())
				player->HandleShooting(30); //returns shooting if lmouse is clicked
			player->ShootAnimation(); //handles shooting animation
			float angleToMouse = mouse->getAngleBetweenMouseAndRect(player->getObject());
			if (player->getActive())
				player->Draw(90 + angleToMouse);


			
			//HANDLE BULLET SPAWNING
			if (player->getShooting()) //if player is shooting
			{
				//instantiate a new temporary pointer to a bullet object
				std::shared_ptr<Bullet> tempBullet{
					new Bullet(window, renderer, "assets/bullet16.png", 0, 0, 16, 16) 
				};
				//player's position + offset half of player's width(to center it relative to player) - offset half of bullet's width(to center it relative to itself)
				int bulletStartPosX = player->getObject().x + player->getObject().w / 2 - tempBullet->getObject().w / 2;
				//player's position + offset half of player's height(to center it relative to player) - offset half of bullet's height(to center it relative to itself)
				int bulletStartPosY = player->getObject().y + player->getObject().h / 2 - tempBullet->getObject().h / 2;
				tempBullet->Fire(bulletStartPosX, bulletStartPosY, mouse->getObject().x, mouse->getObject().y, angleToMouse);
				bulletList.push_back(tempBullet); //transfer ownership of bullet object to the bullet list vector
				tempBullet = nullptr;
				player->setShooting(false); //set shooting to false
			}

			//HANDLE BULLET-ENEMY COLLISION AND BULLET UPDATES
			for (auto bullet_itr = bulletList.begin(); bullet_itr != bulletList.end();) //for every bullet in bulletlist
			{
				if (!(*bullet_itr)->inScreen(SCREEN_WIDTH, SCREEN_HEIGHT)) // if bullet is not in screen
				{
					(*bullet_itr)->setActive(false);
				}
				if ((*bullet_itr)->getActive())
				{
					(*bullet_itr)->Update(1000.0f * time->getDeltaTime()); //update bullet
					(*bullet_itr)->Draw();; //not shooting, so we set the boolean back to false
				}
				for (auto enemy_itr = enemies.begin(); enemy_itr != enemies.end();) // OUTER LOOP(for every bullet) checks against INNER LOOP(for every enemy)
				{
					if (SDL_HasIntersection(&(*bullet_itr)->getObject(), &(*enemy_itr)->getObject()))  //if there is an intersection between current bullet and current enemy
					{
						//save current enemy's position
						SDL_Rect explosionObject = (*enemy_itr)->getObject();
						//instantiate a temporary pointer to an explosion
						std::shared_ptr<AnimationSprite> tempExplosion{
							new AnimationSprite(window, renderer, "assets/explosion1x8.png", explosionObject.x, explosionObject.y, explosionObject.w, explosionObject.h, 1, 8, 3, false) 
						};
						//transfer ownership of explosion object to explosion vector
						explosionList.push_back(tempExplosion);
						tempExplosion = nullptr;
						enemy_itr = enemies.erase(enemy_itr); //erase current enemy from enemy list
						(*bullet_itr)->setActive(false); //set current bullet to inactive
					}
					else //else
						enemy_itr++; //continue iterating through every enemy
				}
				if (!(*bullet_itr)->getActive()) //if current bullet is inactive(collided with enemy or out of screen)
					bullet_itr = bulletList.erase(bullet_itr); //erase bullet from bulletlist
				else
					bullet_itr++; //continue iterating through every bullet

			}


			spawner->setSpawners(enemyCount + 1); //fill up spawnpoints vector
			spawner->setPosition(int(spawner->getSpawnPoint(currentSpawnPoint).x), int(spawner->getSpawnPoint(currentSpawnPoint).y));
			spawner->Animate();
			spawner->Draw();

			//HANDLE EXPLOSIONS
			for (auto explosion_itr = explosionList.begin(); explosion_itr != explosionList.end();) //for every explosion
			{
				if ((*explosion_itr)->getActive()) //if current explosion is active
				{
					//animate and draw it
					(*explosion_itr)->Animate();
					(*explosion_itr)->Draw();
				}
				if (!(*explosion_itr)->getActive())// if current explosion is not active
				{
					explosion_itr = explosionList.erase(explosion_itr);
				}
				else
					explosion_itr++;
			}

			//HANDLE ENEMY SPAWNING
			if (!spawner->getActive() && currentSpawnPoint < enemyCount) //if spawner is not active and current spawnpoint is less than the enemy count
			{
				//instantiate a temporary pointer to an enemy object
				std::shared_ptr<Enemy> tempEnemy{
					new Enemy(window, renderer, "assets/enemy.png", int(spawner->getSpawnPoint(currentSpawnPoint).x), int(spawner->getSpawnPoint(currentSpawnPoint).y), 64, 64) 
				};
				tempEnemy->InitializeWaypoints(3, SCREEN_WIDTH, SCREEN_HEIGHT);	// parameter 1 - how big our waypoints vector is(how many we have access to)
				enemies.push_back(tempEnemy);	// transfer ownership of enemy object to enemy vector
				tempEnemy = nullptr;
				currentSpawnPoint++;
				spawner->setActive(true);
				if (currentSpawnPoint == enemyCount)
					spawner->setActive(false);
			}

			//HANDLE ENEMY UPDATES
			for (auto enemy : enemies)
			{
				enemy->Update(2, time->getDeltaTime() * 0.05f, SCREEN_WIDTH, SCREEN_HEIGHT); //update our enemies
				enemy->Draw(); //draw it

				if (SDL_HasIntersection(&player->getObject(), &enemy->getObject())) //check for player and enemy collision
					player->setActive(false); //if there is, set player inactive
			}


			SDL_RenderPresent(renderer);
			time->reset(); //reset ingame timer
		}
	}
	CloseSDL();
}

bool GameLoop::InitializeSDL()
{
	bool success = true;
	srand(unsigned(time(NULL)));
	//initialize sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		//create window
		window = SDL_CreateWindow("window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			int imageFlag = IMG_INIT_PNG;
			//if imageFlag is created successfully but it can't be used to initialize SDL2_image
			if (!(IMG_Init(imageFlag) & imageFlag))
			{
				std::cout << "SDL Image could not initialize! SDL_image error: " << IMG_GetError() << std::endl;
				success = false;
			}
			else
			{
				//sanity check for debugging
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
				if (renderer == NULL)
				{
					std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
					success = false;
				}
				else
					screenSurface = SDL_GetWindowSurface(window);
			}
		}
	}
	return success;
}
void GameLoop::CloseSDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}