#include "Level.h"
#include "SolidObj.h"

float modulo(float x, int m) {
	int int_r = (int)x % m;
	float fl_r = x - (int)x;
	return int_r + fl_r;
}

Level::Level() {
	MapTexture = { 0,0 };
	mapTextureOffset = { 0,0 };
	mapTextureH = mapTextureW = 0;
	mapH = mapW = 0;
	specialMechanics = 0;
	map = NULL;
	bombs = NULL;
	Player1 = NULL;
	Player2 = NULL;
	AI = NULL;
	AIno = 0;
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
			case SolidObject:
				objTable[SolidObject][instanceCount[SolidObject]++] = new SolidObj({ j * 16,i * 16 }, 16, 16);
				break;
			case ObstacleStatic:
				objTable[ObstacleStatic][instanceCount[ObstacleStatic]++] = new Obstacle1({ j * 16,i * 16 }, 16, 16);
				break;
			case ObstacleAnim:
				objTable[ObstacleAnim][instanceCount[ObstacleAnim]++] = new Obstacle2({ j * 16,i * 16 }, 16, 16);
				break;
			}
		}
	}

	Player1 = new Player({ 18,18 }, 12, 12);
	Player1->range = 2;
	bombs = new Bomb * [2];
	bombs[0] = NULL;
	bombs[1] = NULL;


	MapTexture.x = 320 - mapTextureW;
	MapTexture.y = 240 - mapTextureH;
	MapTexture.w = mapTextureW * 2;
	MapTexture.h = mapTextureH * 2;

	mapTextureOffset = { mapTextureOffset.x * 2 + MapTexture.x,mapTextureOffset.y * 2 + MapTexture.y };

}

int Level::Show() {

	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::LevelMap], NULL, &MapTexture);

	for (int i = 2; i < 5; ++i) {
		for (int j = 0; j < objectCount[i]; ++j) {
			if (objTable[i][j]->Show(mapTextureOffset) == -1) {
				deleteObj(i, j);
			}
		}
	}
	for (int i = 0; i < 2; ++i) {
		if (bombs[i]) {
			int c = bombs[i]->Show(mapTextureOffset);
			if (c == 1) {
				explode(bombs[i]);
			}
			if (c == -1) {
				deleteBomb(bombs[i]);
			}
		}
	}
	if (Player1 != NULL) {
		if (Player1->Show(mapTextureOffset) != 0) {
			delete Player1;
			Player1 = NULL;
			return victoryCheck();
		}
	}
	if (Player2 != NULL) {
		if (Player2->Show(mapTextureOffset) != 0) {
			delete Player2;
			Player2 = NULL;
			return victoryCheck();
		}
	}
	for (int i = 0; i < AIno; ++i) {
		if (AI[i] != NULL) {
			if (AI[i]->Show(mapTextureOffset) != 0) {
				delete AI[i];
				AI[i] = NULL;
				return victoryCheck();
			}
		}
	}
	SDL_RenderPresent(TextureManager::Renderer);
	return 0;
}

float Level::checkCollision(Player* p) {
	int mapX = (int)p->getHitbox().x / 16, mapY = (int)p->getHitbox().y / 16;
	switch (p->getDirection()) {
	case Player::Up:
		if (modulo(p->getHitbox().x, 16) > 4.0f) {
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
		if (modulo(p->getHitbox().x, 16) > 4.0f) {
			if (map[mapY + 1][mapX] == 0 && map[mapY + 1][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 4.0f - modulo(p->getHitbox().y, 16);
			}
		}
		else {
			if (map[mapY + 1][mapX] == 0) {
				return -1.0f;
			}
			else {
				return 4.0f - modulo(p->getHitbox().y, 16);
			}
		}
		break;
	case Player::Left:
		if (modulo(p->getHitbox().y, 16) > 4.0f) {
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
		if (modulo(p->getHitbox().y, 16) > 4.0f) {
			if (map[mapY][mapX + 1] == 0 && map[mapY + 1][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 4.0f - modulo(p->getHitbox().x, 16);
			}
		}
		else {
			if (map[mapY][mapX + 1] == 0) {
				return -1.0f;
			}
			else {
				return 4.0f - modulo(p->getHitbox().x, 16);
			}
		}
		break;
	default:
		return -1.0f;
		break;
	}
	return -1.0f;
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

void Level::placeBomb(Player* p) {
	if (p->placedBombs >= p->maxPlacedBombs) {
		return;
	}
	printf("bomb\n");
	int mapX = (int)p->getHitbox().x / 16, mapY = (int)p->getHitbox().y / 16;
	if (modulo(p->getHitbox().x, 16) >= 8.0f) {
		++mapX;
	}
	if (modulo(p->getHitbox().y, 16) >= 8.0f) {
		++mapY;
	}
	if (bombs[0] != NULL && bombs[1] != NULL) {
		return;
	}
	if (bombs[0] == NULL) {
		++p->placedBombs;
		bombs[0] = new Bomb({ mapX * 16,mapY * 16 }, 16, 16, p);
		map[mapY][mapX] = BombT;
	}
	else {
		++p->placedBombs;
		bombs[0] = new Bomb({ mapX * 16,mapY * 16 }, 16, 16, p);
		map[mapY][mapX] = BombT;
	}

}

void Level::explode(Bomb* b) {
	
	b->setAnimState(Bomb::Explosion);
	int mapX = (int)b->getHitbox().x / 16, mapY = (int)b->getHitbox().y / 16;
	int blastCounter = 0, range = b->getRange();
	Player* rip = checkForPlayer(mapX, mapY);
	if (rip != NULL) {
		rip->isDead = true;
		rip->setAnimState(Player::Defeat);
	}
	Blast** tempArr = new Blast * [range * 4];
	//Up
	for (int i = 1; i <= range; ++i) {
		if (map[mapY - i][mapX] == 1) {
			break;
		}
		else if (map[mapY - i][mapX] == 2 || map[mapY - i][mapX] == 3) {
			for (int j = 0; j < objectCount[2]; ++j) {
				if ((int)objTable[2][j]->getHitbox().x == mapX * 16 && (int)objTable[2][j]->getHitbox().y == (mapY - i) * 16) {
					objTable[2][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			for (int j = 0; j < objectCount[3]; ++j) {
				if ((int)objTable[3][j]->getHitbox().x == mapX * 16 && (int)objTable[3][j]->getHitbox().y == (mapY - i) * 16) {
					objTable[3][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			break;
		}
		else {
			tempArr[blastCounter++] = new Blast({ mapX * 16,(mapY - i) * 16 }, 16, 16, Blast::Up);
			if (i != range) {
				tempArr[blastCounter - 1]->setAnimState(Blast::VerticalMiddle);
			}
			else {
				tempArr[blastCounter - 1]->setAnimState(Blast::VerticalTip);
			}
			rip = checkForPlayer(mapX, mapY - i);
			if (rip != NULL) {
				rip->isDead = true;
				rip->setAnimState(Player::Defeat);
			}
		}
	}
	//Down
	for (int i = 1; i <= range; ++i) {
		if (map[mapY + i][mapX] == 1) {
			break;
		}
		else if (map[mapY + i][mapX] == 2 || map[mapY+i][mapX] == 3) {
			for (int j = 0; j < objectCount[2]; ++j) {
				if ((int)objTable[2][j]->getHitbox().x == mapX * 16 && (int)objTable[2][j]->getHitbox().y == (mapY + i) * 16) {
					objTable[2][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			for (int j = 0; j < objectCount[3]; ++j) {
				if ((int)objTable[3][j]->getHitbox().x == mapX * 16 && (int)objTable[3][j]->getHitbox().y == (mapY + i) * 16) {
					objTable[3][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			break;
		}
		else {
			tempArr[blastCounter++] = new Blast({ mapX * 16,(mapY + i) * 16 }, 16, 16, Blast::Down);
			if (i != range) {
				tempArr[blastCounter - 1]->setAnimState(Blast::VerticalMiddle);
			}
			else {
				tempArr[blastCounter - 1]->setAnimState(Blast::VerticalTip);
			}
		}
		rip = checkForPlayer(mapX, mapY + i);
		if (rip != NULL) {
			rip->isDead = true;
			rip->setAnimState(Player::Defeat);
		}
	}
	//Left
	for (int i = 1; i <= range; ++i) {
		if (map[mapY][mapX - i] == 1) {
			break;
		}
		else if (map[mapY][mapX - i] == 2 || map[mapY][mapX - i] == 3) {
			for (int j = 0; j < objectCount[2]; ++j) {
				if ((int)objTable[2][j]->getHitbox().x == (mapX - i) * 16 && (int)objTable[2][j]->getHitbox().y == mapY * 16) {
					objTable[2][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			for (int j = 0; j < objectCount[3]; ++j) {
				if ((int)objTable[3][j]->getHitbox().x == (mapX - i) * 16 && (int)objTable[3][j]->getHitbox().y == mapY * 16) {
					objTable[3][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			break;
		}
		else {
			tempArr[blastCounter++] = new Blast({ (mapX - i) * 16,mapY * 16 }, 16, 16, Blast::Left);
			if (i != range) {
				tempArr[blastCounter - 1]->setAnimState(Blast::HorizontalMiddle);
			}
			else {
				tempArr[blastCounter - 1]->setAnimState(Blast::HorizontalTip);
			}
			rip = checkForPlayer(mapX - i, mapY);
			if (rip != NULL) {
				rip->isDead = true;
				rip->setAnimState(Player::Defeat);
			}
		}
	}
	//Right
	for (int i = 1; i <= range; ++i) {
		if (map[mapY][mapX + i] == 1) {
			break;
		}
		else if (map[mapY][mapX + i] == 2 || map[mapY][mapX + i] == 3) {
			for (int j = 0; j < objectCount[2]; ++j) {
				if ((int)objTable[2][j]->getHitbox().x == (mapX + i) * 16 && (int)objTable[2][j]->getHitbox().y == mapY * 16) {
					objTable[2][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			for (int j = 0; j < objectCount[3]; ++j) {
				if ((int)objTable[3][j]->getHitbox().x == (mapX + i) * 16 && (int)objTable[3][j]->getHitbox().y == mapY * 16) {
					objTable[3][j]->setAnimState(Obstacle::BlowingUp);
				}
			}
			break;
		}
		else {
			tempArr[blastCounter++] = new Blast({ (mapX + i) * 16,mapY * 16 }, 16, 16, Blast::Right);
			if (i != range) {
				tempArr[blastCounter - 1]->setAnimState(Blast::HorizontalMiddle);
			}
			else {
				tempArr[blastCounter - 1]->setAnimState(Blast::HorizontalTip);
			}
			rip = checkForPlayer(mapX + i, mapY);
			if (rip != NULL) {
				rip->isDead = true;
				rip->setAnimState(Player::Defeat);
			}
		}
	}
	b->setBlast(tempArr, blastCounter);
}

void Level::deleteBomb(Bomb* b) {
	int mapX = (int)b->getHitbox().x / 16, mapY = (int)b->getHitbox().y / 16;
	--b->owner->placedBombs;
	if (b == bombs[0]) {
		delete b;
		bombs[0] = bombs[1];
		bombs[1] = NULL;
	}
	else {
		delete b;
		bombs[1] = NULL;
	}

	map[mapY][mapX] = 0;
}

int Level::victoryCheck() {
	if (Player1 == NULL && Player2 == NULL) {
		return -1;
	}
	if (AI == NULL) {
		return 1;
	}
	return 0;
}

Player* Level::checkForPlayer(int x, int y) {
	int pX, pY;
	for (int i = 0; i < AIno; ++i) {
		if (AI[i] != NULL) {
			pX = (int)AI[i]->getHitbox().x, pY = (int)AI[i]->getHitbox().y;
			if ((pX / 16 == x && pY / 16 == y) || (pX + 12 / 16 == x && pY + 12 / 16 == y) || (pX + 12 / 16 == x && pY / 16 == y) || (pX / 16 == x && pY + 12 / 16 == y)) {
				return AI[i];
			}
		}
	}
	if (Player1 != NULL) {
		pX = (int)Player1->getHitbox().x, pY = (int)Player1->getHitbox().y;
		if ((pX / 16 == x && pY / 16 == y) || (pX + 12 / 16 == x && pY + 12 / 16 == y) || (pX + 12 / 16 == x && pY / 16 == y) || (pX / 16 == x && pY + 12 / 16 == y)) {
			return Player1;
		}
	}
	if (Player2 != NULL) {
		pX = (int)Player2->getHitbox().x, pY = (int)Player2->getHitbox().y;
		if ((pX / 16 == x && pY / 16 == y) || (pX + 12 / 16 == x && pY + 12 / 16 == y) || (pX + 12 / 16 == x && pY / 16 == y) || (pX / 16 == x && pY + 12 / 16 == y)) {
			return Player2;
		}
	}
	return NULL;
}