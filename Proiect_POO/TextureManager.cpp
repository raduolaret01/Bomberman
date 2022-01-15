#include "TextureManager.h"

SDL_Texture* TextureManager::Texture[6] = { NULL };
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
		Logs::logF << "Unable to load image at " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
		return NULL;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
	if (newTexture == NULL) {
		Logs::logF << "Unable to create texture from " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
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
		Logs::logF << "Loading font failed! SLD_ttf Error:" << TTF_GetError() << std::endl;
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