#pragma once

#include <stdio.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_hints.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

enum class TextureEnum {
	Splash,
	Background,
	Player,
	Block,
	Bomb
};

class TextureManager{
public:
	static SDL_Texture* Texture[5];
	static SDL_Renderer* Renderer;

	TextureManager();
	~TextureManager();
	
	static SDL_Texture* loadTexture(const char*);
	static bool loadTextures(void);
	static void freeTextures(void);

};

