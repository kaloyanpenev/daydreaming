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

	Timer* time = new Timer();
	float playerMS = 1;
	Player* littleman = new Player(window, renderer, "assets/shootAnim1x4.png", 500, 500, 64, 64, 1, 4, 5, playerMS);
	Cursor* mouse = new Cursor(window, renderer, "assets/xh.png", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 16, 16);
	AnimationSprite* explosion = new AnimationSprite(window, renderer, "assets/explosion.png", 100, 100, 100, 100, 9, 9, 1, true);
	Spawner* spawners = new Spawner(window, renderer, "assets/spawner1x8.png", 0, 0, 64, 64, 1, 8, 5, false);
	std::vector<Enemy*> enemies;
	int enemyCount = 10;
	int currentSpawnPoint = 0;

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

			spawners->setSpawners(enemyCount + 1); //fill up spawnpoints vector

			spawners->setPosition(int(spawners->getSpawnPoint(currentSpawnPoint).x), int(spawners->getSpawnPoint(currentSpawnPoint).y));
			spawners->Animate();
			spawners->Draw();


			if (spawners->GetDead() == true && currentSpawnPoint < enemyCount)
			{
				std::cout << "HIT!";
				//instantiate a temporary enemy object
				Enemy* temp = new Enemy(window, renderer, "assets/littleman.bmp", int(spawners->getSpawnPoint(currentSpawnPoint).x), int(spawners->getSpawnPoint(currentSpawnPoint).y), 64, 64);
				temp->InitializeWaypoints(3);	//intialize the waypoints that the enemy object is going to follow
				enemies.push_back(temp);	//add the temporary enemy object to the enemy vector
				currentSpawnPoint++;
				spawners->SetDead(false);
				if (currentSpawnPoint == enemyCount)
					spawners->SetDead(true);
			}



			for (Enemy* enemy : enemies)
			{
				enemy->Update(2, time->getDeltaTime() * 0.05);
				enemy->Draw();
			}

			littleman->Move();
			littleman->HandleShooting();
			littleman->ShootAnimation();
			float anglePlayer = mouse->getAngleBetweenMouseAndRect(littleman->getPosition());
			littleman->Draw(90 + anglePlayer);

			mouse->MouseLook(&e);
			mouse->DrawLineMouseToSprite(littleman);
			mouse->Draw();

			explosion->Animate();
			explosion->Draw();

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderPresent(renderer);
			time->reset();
		}
	}
	for (Enemy* en : enemies)
	{
		delete en;
	}
	delete spawners;
	delete explosion;
	delete littleman;
	delete mouse;
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