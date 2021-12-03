#include "Game.h"

Game::Game() {
    gameWindow = NULL;
    gameSurface = NULL;
}

void Game::Init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
        gameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
        if (gameWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        gameSurface = SDL_GetWindowSurface(gameWindow);
        SDL_Surface* img = SDL_LoadBMP("Resources/sus.bmp");
        SDL_BlitScaled(img, NULL, gameSurface, NULL);
        SDL_UpdateWindowSurface(gameWindow);
        SDL_FreeSurface(img);
        img = 0;
        Mix_Chunk* bruh = Mix_LoadWAV("Resources/sus.mp3");
        Mix_PlayChannel(-1, bruh, 0);
        SDL_Delay(4000);
        Mix_FreeChunk(bruh);
        bruh = 0;
    }
}

void Game::Quit(void) {
    SDL_DestroyWindow(gameWindow);
    gameWindow = 0;
    gameSurface = 0;
    SDL_Quit();
}