#include "LevelLoader.h"

LevelLoader::LevelLoader() {
	levelLoaded = false;
}

LevelLoader::~LevelLoader() {
	if (levelFile.is_open()) {
		levelFile.close();
	}
}

bool LevelLoader::loadLevel(Level* level, int levelId) {
	switch (levelId) {
	case sp_1:
		levelFile.open("Resources/level1.txt");
		break;
	case sp_2:
		levelFile.open("Resources/level2.txt");
		break;
	case sp_3:
		levelFile.open("Resources/level3.txt");
		break;
	case mp_coop:
		levelFile.open("Resources/level4.txt");
		break;
	case mp_vs:
		levelFile.open("Resources/level5.txt");
		break;
	default:
		printf("Error: Level not implemented!\n");
		return false;
	}
	if (!levelFile.is_open()) {
		printf("Error opening level file!");
		return false;
	}

	levelFile >> level->mapTextureOffset.x >> level->mapTextureOffset.y;
	
	levelFile >> level->mapTextureW >> level->mapTextureH;

	int w, h;
	levelFile >> h >> w;

	int** a = new int* [h];
	for (int i = 0; i < h; ++i) {
		a[i] = new int[w];
	}

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			levelFile >> a[i][j];
		}
	}

	int smFlag = 0;
	levelFile >> smFlag;

	level->mapH = h;
	level->mapW = w;
	level->map = a;
	level->specialMechanics = smFlag;
	
	char* mapPath = new char[24], * tilesetPath = new char[26];
	
	while (levelFile.peek() == ' ' || levelFile.peek() == '\n') {
		levelFile.get();
	}

	levelFile.get(mapPath, 24, '\n');
	levelFile.get();
	levelFile.get(tilesetPath, 26, '\n');

	TextureManager::Texture[TextureManager::LevelMap] = TextureManager::loadTexture(mapPath); 
	TextureManager::Texture[TextureManager::LevelTileSet] = TextureManager::loadTexture(tilesetPath);

	Obstacle1::loadAnimationStates(&levelFile);
	Obstacle2::loadAnimationStates(&levelFile);

	delete[] mapPath;
	delete[] tilesetPath;
	levelFile.close();
	return true;
}

void LevelLoader::unloadLevel(Level* level) {
	if (TextureManager::Texture[TextureManager::LevelMap]) {
		SDL_DestroyTexture(TextureManager::Texture[TextureManager::LevelMap]);
		TextureManager::Texture[TextureManager::LevelMap] = NULL;
	}
	if (TextureManager::Texture[TextureManager::LevelTileSet]) {
		SDL_DestroyTexture(TextureManager::Texture[TextureManager::LevelTileSet]);
		TextureManager::Texture[TextureManager::LevelTileSet] = NULL;
	}
	level->~Level();
	level = NULL;
	levelLoaded = false;
}