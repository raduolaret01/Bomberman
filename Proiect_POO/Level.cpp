#include "Level.h"
#include "SolidObj.h"

Level::Level() {
	mapTextureOffset = { 0,0 };
	mapTextureH = mapTextureW = 0;
	mapH = mapW = 0;
	specialMechanics = 0;
	map = NULL;
	objectCount = new int[5];
	for (int i = 0; i < 5; ++i) {
		objectCount[i] = 0;
	}
	objTable = new GameObj * *[5];
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
	if (objTable) {
		for (int i = 1; i < 5; ++i) {
			if (objTable[i]) {
				delete[] objTable[i];
			}
		}
		delete[] objTable;
		objTable = NULL;
	}
	if (objectCount) {
		delete[] objectCount;
		objectCount = NULL;
	}
}

void Level::Init() {
	for (int i = 0; i < mapH; ++i) {
		for (int j = 0; j < mapW; ++j) {
			++objectCount[map[i][j]];
		}
	}

	for (int i = 1; i < 5; ++i) {
		objTable[i] = new GameObj * [objectCount[i]];
	}

	int* instanceCount = new int[5];
	for (int i = 0; i < 5; ++i) {
		instanceCount[i] = 0;
	}

	for (int i = 0; i < mapH; ++i) {
		for (int j = 0; j < mapW; ++j) {
			switch (map[i][j]) {
			case 1:
				objTable[map[i][j]][instanceCount[map[i][j]]++] = new SolidObj({ j * 16,i * 16 }, 16, 16);
				break;
			case 2:
				objTable[map[i][j]][instanceCount[map[i][j]]++] = new Obstacle1({ j * 16,i * 16 }, 16, 16);
				break;
			case 3:
				objTable[map[i][j]][instanceCount[map[i][j]]++] = new Obstacle2({ j * 16,i * 16 }, 16, 16);
				break;
			}
		}
	}

	Player1 = new Player({ 0,0 }, 14, 14);

	MapTexture.x = 300 - mapTextureW * 3 / 4;
	MapTexture.y = 200 - mapTextureH * 3 / 4;
	MapTexture.w = mapTextureW * 3 / 2;
	MapTexture.h = mapTextureH * 3 / 2;

	mapTextureOffset = { mapTextureOffset.x * 3 / 2 + MapTexture.x,mapTextureOffset.y * 3 / 2 + MapTexture.y };

}

void Level::Show() {

	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::LevelMap], NULL, &MapTexture);

	for (int i = 2; i < 5; ++i) {
		for (int j = 0; j < objectCount[i]; ++j) {
			if (objTable[i][j]->Show(mapTextureOffset) == -1) {
				deleteObj(i, j);
			}
		}
	}
	Player1->Show(mapTextureOffset);

	SDL_RenderPresent(TextureManager::Renderer);
}

void Level::deleteObj(int i, int j) {
	SDL_Rect pos = objTable[i][j]->getHitbox();
	delete objTable[i][j];
	for (int k = j; k + 1 < objectCount[i]; ++k) {
		objTable[i][k] = objTable[i][k + 1];
	}
	--objectCount[i];
	++objectCount[0];
	map[pos.y/16][pos.x/16] = 0;
}
