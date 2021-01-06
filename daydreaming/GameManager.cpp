#include "GameManager.h"

int GameManager::m_gamemode = gameModes(Survival);


GameManager::GameManager(int _screenWidth, int _screenHeight) :
	SCREEN_WIDTH(_screenWidth),
	SCREEN_HEIGHT(_screenHeight),
	m_restartFlag(false),
	m_gameExitFlag(false),
	m_winFlag(false),
	m_score(0)
{
}


GameManager::~GameManager()
{
}

void GameManager::RunGame()
{
	if (!InitializeSDL())
	{
		std::cout << "Error loading SDL: " << SDL_GetError() << std::endl;
	}
	else
	{
		while (!m_gameExitFlag) //while game is being played
		{
			//initialize menu screen, 
			//Menu() returns true if the Play button is clicked 
			//and false if the game is closed .
			bool menuFlag = Menu();
			if (menuFlag) //if Menu is successfully initialized
			{
				GameLoop(); //run gameLoop
			}
			if (m_restartFlag && menuFlag) //check if player is dead, send to restart screen, check for menuFlag to avoid a bug
			{
				RestartScreen();
				m_score = 0;
			}
		}
	}
	CloseSDL();
}

///Some parts of this function were written by LazyFoo. 
///Available at: http://lazyfoo.net/tutorials/SDL/
bool GameManager::InitializeSDL()
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
		window = SDL_CreateWindow("daydreaming", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN); //SDL_WINDOW_INPUT_GRABBED
		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}

		int imageFlag = IMG_INIT_PNG; // need only PNGs
		//if imageFlag is created successfully but it can't be used to initialize SDL2_image
		if (!(IMG_Init(imageFlag) & imageFlag))
		{
			std::cout << "SDL Image could not initialize! SDL_image error: " << IMG_GetError() << std::endl;
			success = false;
		}
		if (TTF_Init() == -1)
		{
			std::cout << "SDL_TTF could not initialize! SDL_ttf Error: " << TTF_GetError();
			success = false;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
			success = false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL)
		{
			std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
		}
	}
	// load music and play music track
	LoadMusic();
	Mix_PlayMusic(m_musicTrack, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
	SDL_SetWindowGrab(window, SDL_TRUE);

	return success;
}
///end of citation

void GameManager::CloseSDL()
{
	CloseMusic();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
void GameManager::LoadMusic()
{
	//load music
	m_musicTrack = Mix_LoadMUS("assets/lakeyInspired-monroe.mp3");

	if (m_musicTrack == nullptr)
	{
		std::cout << "couldnt load music track" << Mix_GetError();
	}

	m_enemySFX = Mix_LoadWAV("assets/enemySpawn.mp3"); ///available at https://www.zapsplat.com/music/scribbling-on-paper-with-ballpoint-pen-1/
	m_playerDeathSFX = Mix_LoadWAV("assets/playerDeath.mp3"); ///available at: https://www.zapsplat.com/music/pencil-drop-on-table/
	m_shootSFX = Mix_LoadWAV("assets/shooting.mp3"); ///available at: https://www.zapsplat.com/music/cartoon-mouth-pop-6/
}
void GameManager::CloseMusic()
{
	//close music
	Mix_FreeMusic(m_musicTrack);
	m_musicTrack = nullptr;
	Mix_FreeChunk(m_playerDeathSFX);
	m_playerDeathSFX = nullptr;
	Mix_FreeChunk(m_enemySFX);
	m_enemySFX = nullptr;
	Mix_FreeChunk(m_shootSFX);
	m_shootSFX = nullptr;
}
bool GameManager::Menu()
{
	//initialization of the buttons and background
	auto startScreen = std::make_unique<Sprite>(window, renderer, "assets/startScreen.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	auto playButton = std::make_unique<Sprite>(window, renderer, "assets/playbutton.png", SCREEN_WIDTH/2 - 256, SCREEN_HEIGHT/2 - 64, 512, 128);
	auto quitButton = std::make_unique<Sprite>(window, renderer, "assets/quitbutton.png", playButton->getObject().x + 64, playButton->getObject().y + playButton->getObject().h + 32, 384, 96);

	SDL_Color textColor = { 17, 17, 17, 0xFF };
	auto selectionCircle = std::make_unique<Sprite>(window, renderer, "assets/selectBubble.png", 196, 316, 155, 47);
	auto enemiesExtension = std::make_unique<Sprite>(window, renderer, "assets/enemiesExtension.png", 150, 450, 304, 270);
	enemiesExtension->setActive(false);

	auto enemiesTextBox = std::make_unique<Sprite>(window, renderer, "assets/enemies-box.png", 420, 572, 85, 39);
	auto spawnSpeedTextBox = std::make_unique<Sprite>(window, renderer, "assets/enemies-box.png", 420, 612, 85, 39);
	auto enemyInputTextBox = std::make_unique<TextSprite>(window, renderer, "assets/IndieFlower.ttf", 430, 572, 85, 39, 36, m_enemyCountString, textColor);
	enemyInputTextBox->setActive(false);
	auto spawnInputTextBox = std::make_unique<TextSprite>(window, renderer, "assets/IndieFlower.ttf", 430, 610, 85, 39, 36, m_spawnSpeedString, textColor);
	
	spawnInputTextBox->Update(m_spawnSpeedString.c_str());
	enemyInputTextBox->Update(m_enemyCountString.c_str());
	selectionCircle->setActive(false);
	m_gamemode = gameModes(Survival);
	bool inputtingEnemies = false;
	bool inputtingSpeed = false;
	SDL_StopTextInput();

	SDL_Event e;
	//exit flag
	bool exitFlag = false;
	//menu loop, will run until the exit flag breaks it
	while (!exitFlag)
	{
		bool renderInputText = false;

		//poll events
		while (SDL_PollEvent(&e) != 0)
		{
			//if user quits
			if (e.type == SDL_QUIT)
			{
				exitFlag = true;
				m_gameExitFlag = true;
			}
			ToggleMusic(&e);

			if (e.type == SDL_KEYDOWN)
			{
				//Handle backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && m_enemyCountString.length() > 0 && inputtingEnemies)
				{
					//lop off character
					m_enemyCountString.pop_back();
					renderInputText = true;
				}
				else if (e.key.keysym.sym == SDLK_BACKSPACE && m_spawnSpeedString.length() > 0 && inputtingSpeed) 
				{
					//lop off character
					m_spawnSpeedString.pop_back();
					renderInputText = true;
				}

			}
			else if (e.type == SDL_TEXTINPUT)
			{
				//Not copy or pasting
				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')) && inputtingEnemies)
				{
					//Append character
					m_enemyCountString += e.text.text;
					renderInputText = true;
				}
				else if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')) && inputtingSpeed)
				{
					//Append character
					m_spawnSpeedString += e.text.text;
					renderInputText = true;
				}
			}

		}
		SDL_RenderClear(renderer);
		//draw the objects
		startScreen->Draw();
		playButton->Draw();
		quitButton->Draw();

		if(selectionCircle->getActive())
			selectionCircle->Draw();



		//if event is mouse button down
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int MouseX, MouseY;
			//get mouse coordinates
			SDL_GetMouseState(&MouseX, &MouseY);
			//collision detection for the quit button
			if (MouseOverButton(quitButton->getObject(), MouseX, MouseY))
			{
				//global exit flag set to true and local exit flag set to true
				m_gameExitFlag = true;
				exitFlag = true;
			}
			//collision detection for the play button
			if (MouseOverButton(playButton->getObject(), MouseX, MouseY))
			{
				return true;
			}
			//if mouse is over "survival" gamemode
			if (MouseOverButton({ 196, 316, 135, 38 }, MouseX, MouseY))
			{
				if(!selectionCircle->getActive())
					selectionCircle->setActive(true);
				m_gamemode = gameModes(Survival);
				selectionCircle->setPosition(186, 316);
				enemiesExtension->setActive(false);
				SDL_StopTextInput();
			}
			//if mouse is over "time-trial" gamemode
			if (MouseOverButton({ 196, 353, 135, 38 }, MouseX, MouseY))
			{
				if (!selectionCircle->getActive())
					selectionCircle->setActive(true);
				m_gamemode = gameModes(Time_Trial);
				selectionCircle->setPosition(186, 353);
				m_enemyCount = 15;
				enemiesExtension->setActive(false);
				SDL_StopTextInput();
			}
			//if mouse is over "training" gamemode
			if (MouseOverButton({ 196, 390, 135, 38 }, MouseX, MouseY))
			{
				if (!selectionCircle->getActive())
					selectionCircle->setActive(true);
				m_gamemode = gameModes(Training);
				selectionCircle->setPosition(186, 390);
				enemyInputTextBox->setActive(true);
				enemiesExtension->setActive(true);
			}
			//if mouse is over enemiestextbox and the enemies extension text is active
			if (MouseOverButton(enemiesTextBox->getObject(), MouseX, MouseY) && enemiesExtension->getActive())
			{
				SDL_StartTextInput();
				inputtingEnemies = true;
				inputtingSpeed = false;
			}
			if (MouseOverButton(spawnSpeedTextBox->getObject(), MouseX, MouseY) && enemiesExtension->getActive())
			{
				SDL_StartTextInput();
				inputtingEnemies = false;
				inputtingSpeed = true;
			}

		}

		if (renderInputText)
		{
			//Text is not empty
			if (m_enemyCountString != "")
			{
				//Render new text
				enemyInputTextBox->Update(m_enemyCountString.c_str());
			}
			//Text is empty
			else
			{
				//Render space texture
				enemyInputTextBox->Update(" ");
			}
			//Text is not empty
			if (m_spawnSpeedString != "")
			{
				//Render new text
				spawnInputTextBox->Update(m_spawnSpeedString.c_str());
			}
			//Text is empty
			else
			{
				//Render space texture
				spawnInputTextBox->Update(" ");
			}
		}
		if (enemiesExtension->getActive())
		{
			enemiesExtension->Draw();
			enemiesTextBox->Draw();
			spawnSpeedTextBox->Draw();
			enemyInputTextBox->Draw();
			spawnInputTextBox->Draw();
		}
		SDL_RenderPresent(renderer);
	}
	return false;

}
void GameManager::RestartScreen()
{
	//initialize 2 ptrs for background and big button
	std::unique_ptr<Sprite> background = nullptr;
	std::unique_ptr<Sprite> mainButton = nullptr;
	//if game is won
	if (m_winFlag)	
	{
		background = std::make_unique<Sprite>(window, renderer, "assets/winscreen.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		mainButton = std::make_unique<Sprite>(window, renderer, "assets/menubutton.png", SCREEN_WIDTH / 2 - 256, SCREEN_HEIGHT / 2 - 64, 512, 128);
	}
	//if game is lost
	else
	{
		background = std::make_unique<Sprite>(window, renderer, "assets/retryscreen.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		mainButton = std::make_unique<Sprite>(window, renderer, "assets/tryagainbutton.png", SCREEN_WIDTH / 2 - 256, SCREEN_HEIGHT / 2 - 64, 512, 128);
	}
	auto quitButton = std::make_unique<Sprite>(window, renderer, "assets/quitbutton.png", mainButton->getObject().x + 64, mainButton->getObject().y + mainButton->getObject().h + 32, 384, 96);

	SDL_Color score_color{ 25,25,155,250 };
	std::stringstream strstream;
	strstream << "Score: ";
	strstream << m_score;
	std::string scoreResult = strstream.str();
	auto scoreText = std::make_unique<TextSprite>(window, renderer, "assets/IndieFlower.ttf", int(SCREEN_WIDTH / 8), 75, 256, 64, 64, scoreResult, score_color);
	
	if (m_gamemode == Time_Trial)
	{

		scoreText->Update({ "Score: " + std::to_string((m_score / 60) % 60) + " seconds" });
	}
	else
	{
		scoreText->Update(scoreResult);
	}
	SDL_Event e;
	//exit flag
	bool exitFlag = false;
	//menu loop, will run until exitflag breaks it
	while (!exitFlag)
	{
		//poll events
		while (SDL_PollEvent(&e) != 0)
		{
			//if user quits
			if (e.type == SDL_QUIT)
			{
				exitFlag = true;
				m_gameExitFlag = true;
			}
			ToggleMusic(&e);
		}
		SDL_RenderClear(renderer);
		//draw objects

		background->Draw();
		mainButton->Draw();
		quitButton->Draw();
		scoreText->Draw();

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int MouseX, MouseY;
			SDL_GetMouseState(&MouseX, &MouseY);
			//collision detection for the quit button
			if (MouseOverButton(quitButton->getObject(), MouseX, MouseY))
			{
				m_gameExitFlag = true;
				exitFlag = true;
			}
			if (MouseOverButton(mainButton->getObject(), MouseX, MouseY))
			{
				exitFlag = true;
			}
		}
		SDL_RenderPresent(renderer);
	}

}
void GameManager::GameLoop()
{
	int currentSpawnPoint = 0;
	int playerMS = 1;
	float spawningSpeed = 6;
	if (m_gamemode == Survival)
	{
		m_enemyCount = 1000;
	}
	else if (m_gamemode == Time_Trial)
	{
		m_enemyCount = 15;
		spawningSpeed = 0.5f;
	}
	else if (m_gamemode == Training)
	{
		m_enemyCount = stoi(m_enemyCountString);
		spawningSpeed = stof(m_spawnSpeedString) * 6;
	}
	//flag to check if all enemies have been spawned
	bool enemyWaveComplete = false;
	SDL_Color score_color{ 25,25,155,250 };


	//using smart pointers to instantiate some of our game objects
	//unique pointer is used when we will have only 1 instance of the object (e.g. background, player, etc)
	//shared pointer is used when we have a transfer of ownership when working with objects
	//using make_unique for exception-safety

	//create timer object, explicit type definition once to increase readability for the rest of the initializations, will use "auto" for the rest because the type can be inferred from "make_unique"
	std::unique_ptr<Timer> time = std::make_unique<Timer>();
	//create background object
	auto background = std::make_unique<Sprite>(window, renderer, "assets/backgroundLight.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//create player object
	auto player = std::make_unique<Player>(window, renderer, "assets/shootAnim1x4.png", int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 2), 64, 64, 1, 4, 2.0f, playerMS);
	//create mouse object
	auto mouse = std::make_unique<Cursor>(window, renderer, "assets/xh.png", int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 2), 16, 16);
	//spawner initialization, unique because we will be recycling our spawner(don't want enemies spawning at the same time, yet)
	auto spawner = std::make_unique<Spawner>(window, renderer, "assets/spawner1x8.png", 0, 0, 64, 64, 1, 8, spawningSpeed, false);
	spawner->setSpawners(m_enemyCount + 1);


	//create text for score , note: make_unique requires a constructor so i cant uniformly intialize the color or the string, using a hash-define instead.

	auto scoreText = std::make_unique<TextSprite>(window, renderer, "assets/IndieFlower.ttf", int(SCREEN_WIDTH / 8), 30, 256, 64, 64, std::string("Score:"), score_color);
	//create our enemies, explosions and bullet vectors, note: vector of shared pointers because we will be pushing back temporary objects(ownership of objects will get transfered)
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<AnimationSprite>> explosionList;
	std::vector<std::shared_ptr<Bullet>> bulletList;




	//event tracker
	SDL_Event e;
	//exit flag
	bool exitFlag = false;

	//GAME LOOP
	while (!exitFlag)
	{
		time->Update();
		//poll events
		while (SDL_PollEvent(&e) != 0)
		{
			//if user quits
			if (e.type == SDL_QUIT)
			{
				exitFlag = true;
				m_gameExitFlag = true;
				m_restartFlag = false;
			}
			ToggleMusic(&e);
			if (e.type == SDL_KEYDOWN)
			{
				//Handle backspace
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					exitFlag = true;
					m_restartFlag = true;
				}
			}
		}
		if (time->getDeltaTime() >= 1.0f / FRAME_RATE) //limit framerate
		{
			SDL_RenderClear(renderer);
			background->Draw();
			if (m_gamemode == gameModes(Survival))
			{
				m_score++;
				spawner->setSpeed((float)(5000 - m_score) / 1000);
			}
			else if (m_gamemode == gameModes(Time_Trial))
			{
				m_score++;
			}

			//HANDLE SCORETEXT
			std::stringstream scoreStringStream;
			scoreStringStream << m_score;
			std::string currentScore = scoreStringStream.str();
			if (m_gamemode == gameModes(Survival))
			{
				scoreText->Update({ "Score: " + currentScore });
			}
			else if (m_gamemode == gameModes(Time_Trial))
			{
				scoreText->Update({ "Score: " + std::to_string((stoi(currentScore) / 60) % 60) + 's'});
			}
			scoreText->Draw();

			//HANDLE PLAYER INTERACTION
			if (player->getActive())
			{
				player->Move(SCREEN_WIDTH, SCREEN_HEIGHT);
			}
			if (!player->getShooting())
			{
				player->HandleShooting(15); //returns shooting if lmouse is clicked RELOADTIME
			}
			player->ShootAnimation(); //handles shooting animation
			float angleToMouse = mouse->AngleBetweenMouseAndRect(player->getObject());
			if (player->getActive())
			{
				player->Draw(90 + angleToMouse);
			}
			//HANDLE BULLET SPAWNING
			if (player->getShooting()) //if player is shooting
			{
				//instantiate a new temporary pointer to a bullet object
				Mix_PlayChannel(-1, m_shootSFX, 0);
				std::shared_ptr<Bullet> tempBullet{
					std::make_shared<Bullet>(window, renderer, "assets/bullet16.png", 0, 0, 16, 16)
				};
				//player's position + offset half of player's width(to center it relative to player) - offset half of bullet's width(to center it relative to itself)
				int bulletStartPosX = player->getObject().x + player->getObject().w / 2 - tempBullet->getObject().w / 2;
				//player's position + offset half of player's height(to center it relative to player) - offset half of bullet's height(to center it relative to itself)
				int bulletStartPosY = player->getObject().y + player->getObject().h / 2 - tempBullet->getObject().h / 2;
				tempBullet->Fire(bulletStartPosX, bulletStartPosY, mouse->getObject().x, mouse->getObject().y, angleToMouse);
				bulletList.push_back(tempBullet); //transfer ownership of bullet object to the bullet list vector
				//pointer to object is now stored in the container, however tempBullet is still valid so we have to take care of it
				tempBullet = nullptr; //destroy the link to the object so the object can be deleted later on, this pointer will be deleted when it goes out of scope
				player->setShooting(false); //set shooting to falses
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
					(*bullet_itr)->Update(1500.0f * time->getDeltaTime()); //update bullet keywords: bulletspeed, bullet_speed, bulletSpeed, speedBullet
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
							std::make_shared<AnimationSprite>(window, renderer, "assets/explosion1x8.png", explosionObject.x, explosionObject.y, explosionObject.w, explosionObject.h, 1, 8, 3.0f, false)
						};
						//transfer ownership of explosion object to explosion vector
						explosionList.push_back(tempExplosion);
						//pointer to object is now stored in the container, however tempExplosion is still valid so we have to take care of it
						tempExplosion = nullptr; //destroy the link to the object so it can be deleted later on, this pointer will be deleted when it goes out of scope
						enemy_itr = enemies.erase(enemy_itr); //erase current enemy from enemy list
						(*bullet_itr)->setActive(false); //set current bullet to inactive
					}
					else //else
					{
						enemy_itr++; //continue iterating through every enemy
					}
				}
				if (!(*bullet_itr)->getActive()) //if current bullet is inactive(collided with enemy or out of screen)
				{
					bullet_itr = bulletList.erase(bullet_itr); //erase bullet from bulletlist
				}
				else
				{
					bullet_itr++; //continue iterating through every bullet
				}

			}

			spawner->setSpawners(m_enemyCount); //fill up spawnpoints vector
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
				{
					explosion_itr++;
				}
			}
			//HANDLE ENEMY SPAWNING
			if (!spawner->getActive() && currentSpawnPoint < m_enemyCount) //if spawner is not active and current spawnpoint is less than the enemy count
			{
				Mix_PlayChannel(-1, m_enemySFX, 0); //play sound effect
				//instantiate a temporary pointer to an enemy object
				std::shared_ptr<Enemy> tempEnemy{
					std::make_shared<Enemy>(window, renderer, "assets/enemy.png", int(spawner->getSpawnPoint(currentSpawnPoint).x), int(spawner->getSpawnPoint(currentSpawnPoint).y), 64, 64)
				};
				tempEnemy->InitializeWaypoints(3, SCREEN_WIDTH, SCREEN_HEIGHT);	// parameter 1 - how many points it will move to
				enemies.push_back(tempEnemy);	// transfer ownership of enemy object to enemy vector
				//pointer to object is now stored in the container, however tempEnemy is still valid so we have to take care of it
				tempEnemy = nullptr; //destroy the link to the object so it can be deleted later on, this pointer will be deleted when it goes out of scope
				currentSpawnPoint++;
				spawner->setActive(true);
				if (currentSpawnPoint == m_enemyCount)
				{
					spawner->setActive(false);
					enemyWaveComplete = true;
				}
			}
			//HANDLE ENEMY UPDATES AND ENEMY-PLAYER COLLISION
			for (auto enemy : enemies)
			{
				enemy->Update(2, time->getDeltaTime() * 0.05f, SCREEN_WIDTH, SCREEN_HEIGHT); //update our enemies
				enemy->Draw(); //draw it

				if (SDL_HasIntersection(&player->getObject(), &enemy->getObject())) //check for player and enemy collision(if player is dead/died/dying)
				{
					Mix_PlayChannel(-1, m_playerDeathSFX, 0);
					player->setActive(false); //if there is, set player inactive
					m_restartFlag = true;
					exitFlag = true;
				}
			}
			//HANDLE MOUSE
			mouse->MouseLook(&e);
			mouse->DrawLineMouseToSprite(player.get());
			mouse->Draw();

			if (enemyWaveComplete && enemies.empty()) //if all enemies have been spawned and the vector is empty
			{
				//game has been won
				m_winFlag = true;
				exitFlag = true;
				m_restartFlag = true;
			}
			SDL_RenderPresent(renderer);
			time->reset(); //reset ingame timer
		}
	}
}
bool GameManager::MouseOverButton(SDL_Rect _button, int _mouseX, int _mouseY)
{
	if (_mouseX > _button.x &&
		_mouseY > _button.y &&
		_mouseX < _button.x + _button.w &&
		_mouseY < _button.y + _button.h)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void GameManager::ToggleMusic(SDL_Event* _e)
{
	if (_e->type == SDL_KEYDOWN)
	{
		if (_e->key.keysym.sym == SDLK_m)
		{
			if (Mix_PausedMusic() == 1)
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
		}
	}
}