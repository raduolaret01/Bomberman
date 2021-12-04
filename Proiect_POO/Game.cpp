#include "Game.h"

Game::Game() {
	gameWindow = NULL;
	errorFlag = 0;
}

Game::~Game() {
	Quit();
}

bool Game::getErrorFlag(void) {
	return errorFlag;
}

void Game::setErrorFlag(bool flag) {
	errorFlag = flag;
}

void Game::Init(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		errorFlag = 1;
		return;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("IMG library could not initialize! SDL_Error: %s\n", SDL_GetError());
		errorFlag = 1;
		return;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		errorFlag = 1;
		return;
	}
	gameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	if (gameWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		errorFlag = 1;
		return;
	}

	SDL_Renderer* Renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
	if (Renderer == NULL) {
		printf("Renderere could not be created! SDL_Error: %s\n", SDL_GetError());
		errorFlag = 1;
		return;
	}
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	TextureManager::Renderer = Renderer;
	TextureManager::loadTextures();

	SDL_RenderCopy(Renderer, TextureManager::Texture[(int)TextureEnum::Splash], NULL, NULL);
	SDL_RenderPresent(Renderer);

	SoundManager::loadBGM();
	SoundManager::loadSFX();

	Mix_PlayChannel(-1, SoundManager::SFX[(int)SFXEnum::Splash], 0);
	SDL_Delay(4000);

}

void Game::Quit(void) {
	SDL_DestroyRenderer(TextureManager::Renderer);
	TextureManager::Renderer = NULL;
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;
	TextureManager::freeTextures();
	SoundManager::freeSounds();
	IMG_Quit();
	SDL_Quit();
}