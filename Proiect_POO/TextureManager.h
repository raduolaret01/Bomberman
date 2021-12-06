#pragma once

#include <stdio.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_hints.h>
#include <SDL_ttf.h>

class TextureManager{
public:
	 enum TextureEnum {
		Splash,
		Background,
		Player,
		Block,
		Bomb
	};

	enum FontEnum {
		Menu,
		BombCounter,
		SmallText
	};

	static SDL_Texture* Texture[5];
	static TTF_Font* Font[3];
	static SDL_Renderer* Renderer;

	TextureManager();
	~TextureManager();
	
	static SDL_Texture* loadTexture(const char*);
	static bool loadTextures(void);
	static void freeTextures(void);

	static TTF_Font* loadFont(const char*,int);
	static bool loadFonts(void);
	static void freeFonts(void);
};

