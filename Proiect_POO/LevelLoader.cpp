#include "LevelLoader.h"

LevelLoader::LevelLoader() {
	levelFile = NULL;
}

LevelLoader::~LevelLoader() {
	if (levelFile) {
		SDL_RWclose(levelFile);
		levelFile = NULL;
	}
}

bool LevelLoader::loadLevel(Level* level, int levelId) {
	switch (levelId) {
	case sp_1:
		levelFile = SDL_RWFromFile("Resources/level1.txt", "r");
		break;
	case sp_2:
		levelFile = SDL_RWFromFile("Resources/level2.txt", "r");
		break;
	case sp_3:
		levelFile = SDL_RWFromFile("Resources/level3.txt", "r");
		break;
	case mp_coop:
		levelFile = SDL_RWFromFile("Resources/level4.txt", "r");
		break;
	case mp_vs:
		levelFile = SDL_RWFromFile("Resources/level5.txt", "r");
		break;
	default:
		printf("Error: Level not implemented!\n");
		return false;
	}
	if (levelFile == NULL) {
		printf("Error opening level file! SDL error: %s\n", SDL_GetError());
		return false;
	}
	int w, h;
	char* buffer = new char[200];
	SDL_RWread(levelFile, &h, sizeof(int), 1);
	SDL_RWread(levelFile, buffer, sizeof(char), 1);
	SDL_RWread(levelFile, &w, sizeof(int), 1);

	SDL_RWread(levelFile, buffer, sizeof(char), 1);

	int** a = new int* [h];
	for (int i = 0; i < h; ++i) {
		a[i] = new int[w];
	}

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			SDL_RWread(levelFile, a[i]+j, sizeof(int), 1);
			SDL_RWread(levelFile, buffer, sizeof(char), 1);
		}
	}
	SDL_RWread(levelFile, buffer, sizeof(char), 1);

	int smFlag = 0;
	SDL_RWread(levelFile, &smFlag, sizeof(int), 1);
	SDL_RWread(levelFile, buffer, sizeof(char), 1);

	level->mapH = h;
	level->mapW = w;
	level->map = a;
	level->specialMechanics = smFlag;
	
	char* mapPath = new char[24], * tilesetPath = new char[26];
	
	SDL_RWread(levelFile, buffer, sizeof(char), 1);
	SDL_RWread(levelFile, mapPath, 23 * sizeof(char), 1);
	SDL_RWread(levelFile, buffer, sizeof(char), 1);
	SDL_RWread(levelFile, tilesetPath, 26 * sizeof(char), 1);

	TextureManager::Texture[TextureManager::LevelMap] = TextureManager::loadTexture(mapPath); 
	TextureManager::Texture[TextureManager::LevelTileSet] = TextureManager::loadTexture(tilesetPath);

	delete[] mapPath;
	delete[] tilesetPath;
	delete[] buffer;
	SDL_RWclose(levelFile);
	return true;
}