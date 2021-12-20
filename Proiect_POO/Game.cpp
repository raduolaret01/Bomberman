#include "Game.h"

Game::Game() {
	gameWindow = NULL;
	quitFlag = false;
	Menus[Main] = new MainMenu;
	Menus[Pause] = new PauseMenu;
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
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		quitFlag = 1;
		return;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("IMG library could not initialize! SDL_Error: %s\n", SDL_GetError());
		quitFlag = 1;
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		quitFlag = 1;
		return;
	}
	if (TTF_Init() < 0) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return;
	}
	gameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (gameWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		quitFlag = 1;
		return;
	}

	SDL_Renderer* Renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == NULL) {
		printf("Renderere could not be created! SDL_Error: %s\n", SDL_GetError());
		quitFlag = 1;
		return;
	}
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	TextureManager::Renderer = Renderer;
	TextureManager::loadTextures();
	TextureManager::loadFonts();

	SDL_RenderCopy(Renderer, TextureManager::Texture[TextureManager::Splash], NULL, NULL);
	SDL_RenderPresent(Renderer);

	SoundManager::loadBGM();
	SoundManager::loadSFX();

	Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Splash], 0);
	SDL_Delay(4000);
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(TextureManager::Renderer);

	Menus[Main]->Init();
	Menus[Pause]->Init();
	Menus[Main]->Show();
	Menu::setMainMenuFlag();

}

void Game::Quit(void) {
	Menus[Main]->Destroy();
	Menus[Pause]->Destroy();
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