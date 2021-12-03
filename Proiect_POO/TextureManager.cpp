#include "TextureManager.h"

SDL_Texture* TextureManager::Texture[5] = { NULL,NULL,NULL,NULL,NULL };
SDL_Renderer* TextureManager::Renderer = NULL;

TextureManager::TextureManager() {
	return;
}


TextureManager::~TextureManager() {
	for (int i = 0; i < 4; ++i) {
		if (Texture[i])
			SDL_DestroyTexture(Texture[i]);
		Texture[i] = NULL;
	}
}

bool TextureManager::loadTextures(void) {
	Texture[(int)TextureEnum::Splash] = loadTexture("Resources/sus.png");
	if (Texture[(int)TextureEnum::Splash] == NULL) {
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
		printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}