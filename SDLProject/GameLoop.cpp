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
	Player* littleman = new Player(window, renderer, "littleman.bmp", 1280/2, 720/2, 100, 100, 5);
	Cursor* mouse = new Cursor(window, renderer, "xh.png", 0, 0, 16, 16);

	SDL_Event e;
	bool quit = false;


	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		//Clear screen
		SDL_RenderClear(renderer);


		mouse->MouseLook(&e);
		mouse->Draw();
		littleman->Move();
		float angle = mouse->getAngleBetweenMouseAndRect(littleman->getPosition());
		littleman->Draw(90 + angle);
		mouse->DrawLineMouseToSprite(littleman);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);



		SDL_RenderPresent(renderer);
	}
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