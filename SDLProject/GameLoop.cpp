#include "GameLoop.h"


GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::RunGame()
{
	srand(unsigned(time(NULL)));
	if (!InitializeSDL())
		std::cout << "Error loading SDL: " << SDL_GetError() << std::endl;

	Timer* time = new Timer();
	float playerMS = 1;
	Player* littleman = new Player(window, renderer, "assets/shootAnim1x4.png", 500, 500, 64, 64, 1, 4, 5, playerMS);
	Cursor* mouse = new Cursor(window, renderer, "assets/xh.png", 0, 0, 16, 16);
	AnimationSprite* explosion = new AnimationSprite(window, renderer, "assets/explosion.png", 100, 100, 100, 100, 9, 9, 1, true);
	Enemy* enemy = new Enemy(window, renderer, "assets/littleman.bmp", 200, 200, 100, 100);

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
		//Clear screen
		if (time->getDeltaTime() >= 1.0f / FRAME_RATE)
		{
			SDL_RenderClear(renderer);

			enemy->Update(10 * time->getDeltaTime());

			enemy->Draw();

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


			std::cout << time->getDeltaTime() << std::endl;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderPresent(renderer);
			time->reset();
		}
	}
	delete enemy;
	delete explosion;
	delete littleman;
	delete mouse;
	CloseSDL();
}

bool GameLoop::InitializeSDL()
{
	bool success = true;

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