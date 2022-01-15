#include "Game.h"

constexpr auto SCREEN_HEIGHT = 480;

constexpr auto SCREEN_WIDTH = 640;


Game::Game() {
	gameWindow = NULL;
	quitFlag = false;
	Menus[Main] = new MainMenu;
	Menus[Pause] = new PauseMenu;
	Menus[GameOverScreen] = new GameOverMenu;
	Menus[VictoryScreen] = new VictoryMenu;
	level = new Level;
}

Game::~Game() {
	Quit();
}

bool Game::getQuitFlag() {
	return quitFlag;
}

void Game::setQuitFlag() {
	quitFlag = true;
}

void Game::Init(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		Logs::logF << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		quitFlag = 1;
		return;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		Logs::logF << "IMG library could not initialize! SDL_Error: "<< SDL_GetError() << std::endl;
		quitFlag = 1;
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		Logs::logF << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		quitFlag = 1;
		return;
	}
	Mix_AllocateChannels(32);
	Mix_Volume(-1, 64);
	if (TTF_Init() < 0) {
		Logs::logF << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return;
	}
	gameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gameWindow == NULL) {
		Logs::logF << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << std::endl;
		quitFlag = 1;
		return;
	}

	SDL_Renderer* Renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == NULL) {
		Logs::logF << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		quitFlag = 1;
		return;
	}
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	TextureManager::Renderer = Renderer;
	TextureManager::loadTextures();
	TextureManager::loadFonts();


	SoundManager::loadBGM();
	SoundManager::loadSFX();

	//SDL_RenderCopy(Renderer, TextureManager::Texture[TextureManager::Splash], NULL, NULL);
	//SDL_RenderPresent(Renderer);
	//Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Splash], 0);
	//SDL_Delay(4000);
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(TextureManager::Renderer);

	Menus[Main]->Init();
	Menus[Pause]->Init();
	Menus[GameOverScreen]->Init();
	Menus[VictoryScreen]->Init();
	Menus[Main]->Show();
	Menu::setMainMenuFlag();
	std::srand(SDL_GetTicks());
}

void Game::Quit(void) {
	Menus[Main]->Destroy();
	Menus[Pause]->Destroy();
	Menus[VictoryScreen]->Destroy();
	Menus[GameOverScreen]->Destroy();
	SDL_DestroyRenderer(TextureManager::Renderer);
	TextureManager::Renderer = NULL;
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;
	TextureManager::freeTextures();
	TextureManager::freeFonts();
	SoundManager::freeSounds();
	IMG_Quit();
	SDL_Quit();
}