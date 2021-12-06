#include "TextureManager.h"

SDL_Texture* TextureManager::Texture[5] = { NULL };
SDL_Renderer* TextureManager::Renderer = NULL;
TTF_Font* TextureManager::Font[3] = { NULL };

TextureManager::TextureManager() {
	return;
}

TextureManager::~TextureManager() {
	for (int i = 0; i < 4; ++i) {
		if (Texture[i])
			SDL_DestroyTexture(Texture[i]);
		Texture[i] = NULL;
	}
	for (int i = 0; i < 3; ++i) {
		if (Font[i])
			TTF_CloseFont(Font[i]);
		Font[i] = NULL;
	}
	if (Renderer)
		SDL_DestroyRenderer(Renderer);
	Renderer = NULL;
}

bool TextureManager::loadTextures(void) {
	Texture[Splash] = loadTexture("Resources/sus.png");
	if (Texture[Splash] == NULL) {
		return false;
	}
	return true;
}

SDL_Texture* TextureManager::loadTexture(const char* path) {
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
	if (newTexture == NULL) {
		printf("Unable to create texture from %s! SDL_image Error: %s\n", path, IMG_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

void TextureManager::freeTextures() {
	for (int i = 0; i < 4; ++i) {
		if (Texture[i])
			SDL_DestroyTexture(Texture[i]);
		Texture[i] = NULL;
	}
	if (Renderer)
		SDL_DestroyRenderer(Renderer);
	Renderer = NULL;
}

TTF_Font* TextureManager::loadFont(const char* path,int pxSize) {
	TTF_Font* newFont = TTF_OpenFont(path, pxSize);
	if (newFont == NULL) {
		printf("Loading font failed! SLD_ttf Error: %s\n", TTF_GetError());
	}
	return newFont;
}

bool TextureManager::loadFonts() {
	Font[(int)FontEnum::Menu] = loadFont("Resources/Lato-Regular.ttf", 30);
	if (Font[Menu] == NULL) {
		return false;
	}
	Font[(int)FontEnum::BombCounter] = loadFont("Resources/FFF_Tusj.ttf", 20);
	if (Font[BombCounter] == NULL) {
		return false;
	}
	Font[(int)FontEnum::SmallText] = loadFont("Resources/Walkway_Bold.ttf", 6);
	if (Font[SmallText] == NULL) {
		return false;
	}
	return true;
}

void TextureManager::freeFonts() {
	for (int i = 0; i < 3; ++i) {
		if (Font[i])
			TTF_CloseFont(Font[i]);
		Font[i] = NULL;
	}
}