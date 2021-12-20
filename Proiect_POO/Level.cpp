#include "Level.h"
#include "SolidObj.h"

float modulo(float x, int m) {
	int int_r = (int)x % m;
	float fl_r = x - (int)x;
	return int_r + fl_r;
}

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

	Player1 = new Player({ 16,16 }, 14, 14);

	MapTexture.x = 320 - mapTextureW;
	MapTexture.y = 240 - mapTextureH;
	MapTexture.w = mapTextureW * 2;
	MapTexture.h = mapTextureH * 2;

	mapTextureOffset = { mapTextureOffset.x * 2 + MapTexture.x,mapTextureOffset.y * 2 + MapTexture.y };

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
	int posX = (int)objTable[i][j]->getHitbox().x, posY = (int)objTable[i][j]->getHitbox().y;
	for (int k = j; k + 1 < objectCount[i]; ++k) {
		objTable[i][k] = objTable[i][k + 1];
	}
	--objectCount[i];
	++objectCount[0];
	map[posY / 16][posX / 16] = 0;
}

float Level::checkCollision(Player* p) {
	int mapX = (int)p->getHitbox().x / 16, mapY = (int)p->getHitbox().y / 16;
	switch (p->getDirection()) {
	case Player::Up:
		if (modulo(p->getHitbox().x, 16) > 2.0f) {
			if (map[mapY - 1][mapX] == 0 && map[mapY - 1][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return modulo(p->getHitbox().y, 16);
			}
		}
		else {
			if (map[mapY - 1][mapX] == 0) {
				return -1.0f;
			}
			else {
				return modulo(p->getHitbox().y, 16);
			}
		}
		break;
	case Player::Down:
		if (modulo(p->getHitbox().x, 16) > 2.0f) {
			if (map[mapY + 1][mapX] == 0 && map[mapY + 1][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 2.0f - modulo(p->getHitbox().y, 16);
			}
		}
		else {
			if (map[mapY + 1][mapX] == 0) {
				printf("1\n");
				return -1.0f;
			}
			else {
				return 2.0f - modulo(p->getHitbox().y, 16);
			}
		}
		break;
	case Player::Left:
		if (modulo(p->getHitbox().y, 16) > 2.0f) {
			if (map[mapY][mapX - 1] == 0 && map[mapY + 1][mapX - 1] == 0) {
				return -1.0f;
			}
			else {
				return modulo(p->getHitbox().x, 16);
			}
		}
		else {
			if (map[mapY][mapX - 1] == 0) {
				return -1.0f;
			}
			else {
				return modulo(p->getHitbox().x, 16);
			}
		}
		break;
	case Player::Right:
		if (modulo(p->getHitbox().y, 16) > 2.0f) {
			if (map[mapY][mapX + 1] == 0 && map[mapY + 1][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 2.0f - modulo(p->getHitbox().x, 16);
			}
		}
		else {
			if (map[mapY][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 2.0f - modulo(p->getHitbox().x, 16);
			}
		}
		break;
	default:
		return -1.0f;
		break;
	}
	return -1.0f;
}