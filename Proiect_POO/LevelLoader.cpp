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
		Logs::logF << "Error: Level not implemented!\n";
		return false;
	}
	if (!levelFile.is_open()) {
		Logs::logF << "Error opening level file!";
		return false;
	}

	levelFile >> level->mapTextureOffset.x >> level->mapTextureOffset.y;
	
	levelFile >> level->mapTextureW >> level->mapTextureH;

	levelFile >> level->mapH >> level->mapW;

	int** a = new int* [level->mapH];
	for (int i = 0; i < level->mapH; ++i) {
		a[i] = new int[level->mapW];
	}

	for (int i = 0; i < level->mapH; ++i) {
		for (int j = 0; j < level->mapW; ++j) {
			levelFile >> a[i][j];
		}
	}

	levelFile >> level->specialMechanics;

	level->map = a;

	levelFile >> level->playerNo >> level->AIno;
	
	char* mapPath = new char[24], * tilesetPath = new char[26];
	
	while (levelFile.peek() == ' ' || levelFile.peek() == '\n') {
		levelFile.get();
	}

	levelFile.get(mapPath, 24, '\n');
	levelFile.get();
	levelFile.get(tilesetPath, 26, '\n');

	if (!(TextureManager::Texture[TextureManager::LevelMap] = TextureManager::loadTexture(mapPath))) {
		return false;
	}

	if (!(TextureManager::Texture[TextureManager::LevelTileSet] = TextureManager::loadTexture(tilesetPath))) {
		return false;
	}

	Obstacle1::loadAnimationStates(&levelFile);
	Obstacle2::loadAnimationStates(&levelFile);

	delete[] mapPath;
	delete[] tilesetPath;
	levelFile.close();

	//load player sprite sheet
	std::ifstream playerFile("Resources/player1.txt"); 
	if (!playerFile.is_open()) {
		Logs::logF << "Error opening player info file!\n";
		return false;
	}

	if (!(TextureManager::Texture[TextureManager::Player1] = TextureManager::loadTexture("Resources/p1sprites.png"))) {
		return false;
	}
	
	Player::loadAnimationStates(&playerFile);

	playerFile.close();

	playerFile.open("Resources/player1.txt");
	if (!playerFile.is_open()) {
		Logs::logF << "Error opening AI player info file!\n";
		return false;
	}

	if (!(TextureManager::Texture[TextureManager::PlayerAI] = TextureManager::loadTexture("Resources/AIpsprites.png"))) {
		return false;
	}

	AIplayer::loadAnimationStates(&playerFile);
	playerFile.close();

	std::ifstream bombFile("Resources/bomb.txt");
	if (!bombFile.is_open()) {
		Logs::logF<<"Error opening bomb info file!\n";
		return false;
	}

	if (!(TextureManager::Texture[TextureManager::BombTileSet] = TextureManager::loadTexture("Resources/bombtileset.png"))) {
		return false;
	}

	Bomb::loadAnimationStates(&bombFile);
	Blast::loadAnimationStates(&bombFile);

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
	if (TextureManager::Texture[TextureManager::Player1]) {
		SDL_DestroyTexture(TextureManager::Texture[TextureManager::Player1]);
		TextureManager::Texture[TextureManager::Player1] = NULL;
	}
	if (TextureManager::Texture[TextureManager::BombTileSet]) {
		SDL_DestroyTexture(TextureManager::Texture[TextureManager::BombTileSet]);
		TextureManager::Texture[TextureManager::BombTileSet] = NULL;
	}
	if (TextureManager::Texture[TextureManager::PlayerAI]) {
		SDL_DestroyTexture(TextureManager::Texture[TextureManager::PlayerAI]);
		TextureManager::Texture[TextureManager::PlayerAI] = NULL;
	}

	Player::unloadAnimationStates();
	AIplayer::unloadAnimationStates();
	Obstacle1::unloadAnimationStates();
	Obstacle2::unloadAnimationStates();
	Bomb::unloadAnimationStates();
	Blast::unloadAnimationStates();

	level->~Level();
	level = NULL;
	levelLoaded = false;
}