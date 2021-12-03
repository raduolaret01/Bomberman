#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_hints.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdio.h>

class Game {
private:
	SDL_Window* gameWindow;
	SDL_Surface* gameSurface;
public:
	Game();
	void Init(void);
	void Quit(void);
};