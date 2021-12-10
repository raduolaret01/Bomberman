#include "Level.h"

Level::Level() {
	mapH = mapW = 0;
	specialMechanics = 0;
	map = NULL;
	objectCount = NULL;
}

Level::~Level() {
	if (map) {
		for (int i = 0; i < mapH; ++i) {
			if (map[i]) {
				delete[] map[i];
				map[i] = NULL;
			}
		}
		delete[] map;
		map = NULL;
	}
	if (objectCount) {
		delete[] objectCount;
		objectCount = NULL;
	}
}

void Level::Show() {
	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::LevelMap], NULL, NULL);
	SDL_RenderPresent(TextureManager::Renderer);
}
