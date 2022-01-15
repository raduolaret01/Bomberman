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
	playerNo = 0;
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
	if (Player1) {
		delete Player1;
		Player1 = NULL;
	}
	if (Player2) {
		delete Player2;
		Player2 = NULL;
	}
	if (AI) {
		delete[] AI;
		AI = NULL;
	}
	if (bombs) {
		delete[] bombs;
		bombs = NULL;
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
	if (playerNo == 2) {
		Player2 = new Player({ 18,178 }, 12, 12);
		Player2->range = 2;
	}
	AI = new AIplayer * [AIno];
	if (AIno >= 1) {
		AI[0] = new AIplayer({ 210,18 }, 12, 12);
	}
	if (AIno >= 2) {
		AI[1] = new AIplayer({ 210,178 }, 12, 12);
	}
	if (AIno == 3) {
		AI[2] = new AIplayer({ 18,178 }, 12, 12);
	}
	bombs = new Bomb * [4];
	for (int i = 0; i < 4; ++i) {
		bombs[i] = NULL;
	}


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
	for (int i = 0; i < 4; ++i) {
		if (bombs[i]) {
			int c = bombs[i]->Show(mapTextureOffset);
			if (c == 1) {
				explode(bombs[i]);
			}
			if (c == -1) {
				deleteBomb();
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
	int mapX = (int)p->getHitbox().x / 16, mapY = (int)p->getHitbox().y / 16;
	if (modulo(p->getHitbox().x, 16) >= 8.0f) {
		++mapX;
	}
	if (modulo(p->getHitbox().y, 16) >= 8.0f) {
		++mapY;
	}
	int i;
	for (i = 0; i < 4 && bombs[i] != NULL; ++i);
	if (i == 4) {
		return;
	}
	Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Bomb_Place], 0);
	bombs[i] = new Bomb({ mapX * 16,mapY * 16 }, 16, 16, p);
	map[mapY][mapX] = BombT;
	++p->placedBombs;

}

void Level::explode(Bomb* b) {
	b->setAnimState(Bomb::Explosion);
	Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Bomb_Boom], 0);
	int mapX = (int)b->getHitbox().x / 16, mapY = (int)b->getHitbox().y / 16;
	int blastCounter = 0, range = b->getRange();
	Player* rip = checkForPlayer(mapX, mapY);
	if (rip != NULL) {
		rip->isDead = true;
		rip->setAnimState(Player::Defeat);
		if (dynamic_cast<AIplayer*>(rip)) {
			Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::NPC_Death], 0);
		}
		else {
			Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Player_Death], 0);
		}
	}
	int tempArrSize = range * 4;
	Blast** tempArr = new Blast * [tempArrSize];
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
				if (dynamic_cast<AIplayer*>(rip)) {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::NPC_Death], 0);
				}
				else {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Player_Death], 0);
				}
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
			if (dynamic_cast<AIplayer*>(rip)) {
				Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::NPC_Death], 0);
			}
			else {
				Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Player_Death], 0);
			}
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
				if (dynamic_cast<AIplayer*>(rip)) {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::NPC_Death], 0);
				}
				else {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Player_Death], 0);
				}
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
				if (dynamic_cast<AIplayer*>(rip)) {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::NPC_Death], 0);
				}
				else {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Player_Death], 0);
				}
			}
		}
	}
	b->setBlast(tempArr, blastCounter);
}

void Level::deleteBomb() {
	int mapX = (int)bombs[0]->getHitbox().x / 16, mapY = (int)bombs[0]->getHitbox().y / 16;
	--bombs[0]->owner->placedBombs;
	for (int i = 0; i < 3; ++i) {
		bombs[i] = bombs[i + 1];
	}
	bombs[3] = NULL;
	
	map[mapY][mapX] = 0;
}

int Level::victoryCheck() {
	if (Player1 == NULL && Player2 == NULL) {
		return -1;
	}
	int vic = 1;
	for (int i = 0; i < AIno; ++i) {
		if (AI[i] != NULL) {
			vic = 0;
		}
	}
	return vic;
}

Player* Level::checkForPlayer(int x, int y) {
	int pX, pY;
	for (int i = 0; i < AIno; ++i) {
		if (AI[i] != NULL) {
			pX = (int)AI[i]->getHitbox().x, pY = (int)AI[i]->getHitbox().y;
			if ((pX / 16 == x && pY / 16 == y) || ((pX + 12) / 16 == x && (pY + 12) / 16 == y) || ((pX + 12) / 16 == x && pY / 16 == y) || (pX / 16 == x && (pY + 12) / 16 == y)) {
				return AI[i];
			}
		}
	}
	if (Player1 != NULL) {
		pX = (int)Player1->getHitbox().x, pY = (int)Player1->getHitbox().y;
		if ((pX / 16 == x && pY / 16 == y) || ((pX + 12) / 16 == x && (pY + 12) / 16 == y) || ((pX + 12) / 16 == x && pY / 16 == y) || (pX / 16 == x && (pY + 12) / 16 == y)) {
			return Player1;
		}
	}
	if (Player2 != NULL) {
		pX = (int)Player2->getHitbox().x, pY = (int)Player2->getHitbox().y;
		if ((pX / 16 == x && pY / 16 == y) || ((pX + 12) / 16 == x && (pY + 12) / 16 == y) || ((pX + 12) / 16 == x && pY / 16 == y) || (pX / 16 == x && (pY + 12) / 16 == y)) {
			return Player2;
		}
	}
	return NULL;
}

void Level::AIDecisionMaking(AIplayer* ai) {
	if (ai->isDead) {
		return;
	}
	ai->reactionTime += Timer::getDTime();
	if ((modulo(ai->getHitbox().x, 16) > 4.0f || modulo(ai->getHitbox().y, 16) > 4.0f) || (ai->reactionTime < ai->reactionDelay && ai->state == AIplayer::Default)) {
		ai->Update(Timer::getDTime(), checkCollision(ai), false, false);
		return;
	}
	ai->reactionTime = 0;
	bool* validDirection = new bool[4];

	for (int i = 0; i < 4; ++i) {
		validDirection[i] = false;
	}

	int mapX = (int)ai->getHitbox().x / 16, mapY = (int)ai->getHitbox().y / 16;

	if (isInRangeOfBomb(mapX, mapY)) {
		ai->state = AIplayer::InRangeOfBomb;
		//"Pathfinding" towards a safe tile in a 5 tile "radius"
		//Up
		for (int i = 1; i < 6 && !validDirection[Player::Up] && mapY - i > 0; ++i) {
			if (map[mapY - i][mapX] == 0) {
				if (!isInRangeOfBomb(mapX, mapY - i)) {
					validDirection[Player::Up] = true;
					break;
				}
				else {
					for (int j = 1; i + j < 6 && !validDirection[Player::Up] && mapX - j > 0; ++j) {
						if (map[mapY - i][mapX - j] == 0) {
							if (!isInRangeOfBomb(mapX - j, mapY - i)) {
								validDirection[Player::Up] = true;
								break;
							}
							else if (i + j < 5) {
								if (map[mapY - i + 1][mapX - j] == 0 && !isInRangeOfBomb(mapX - j, mapY - i + 1)) {
									validDirection[Player::Up] = true;
									break;
								}
								if (mapY - i - 1 > 0 && map[mapY - i - 1][mapX - j] == 0 && !isInRangeOfBomb(mapX - j, mapY - i - 1)) {
									validDirection[Player::Up] = true;
									break;
								}
							}
						}
						else break;
					}
					for (int j = 1; i + j < 6 && !validDirection[Player::Up] && mapX + j < mapW; ++j) {
						if (map[mapY - i][mapX + j] == 0) {
							if (!isInRangeOfBomb(mapX + j, mapY - i)) {
								validDirection[Player::Up] = true;
								break;
							}
							else if (i + j < 5) {
								if (map[mapY - i + 1][mapX + j] == 0 && !isInRangeOfBomb(mapX + j, mapY - i + 1)) {
									validDirection[Player::Up] = true;
									break;
								}
								if (mapY - i - 1 > 0 && map[mapY - i - 1][mapX + j] == 0 && !isInRangeOfBomb(mapX + j, mapY - i - 1)) {
									validDirection[Player::Up] = true;
									break;
								}
							}
						}
						else break;
					}
				}
			}
			else {
				break;
			}
			
		}
		//Down
		for (int i = 1; i < 6 && !validDirection[Player::Down] && mapY + i < mapH; ++i) {
			if (map[mapY + i][mapX] == 0) {
				if (!isInRangeOfBomb(mapX, mapY + i)) {
					validDirection[Player::Down] = true;
					break;
				}
				else {
					for (int j = 1; i + j < 6 && !validDirection[Player::Down] && mapX - j>0; ++j) {
						if (map[mapY + i][mapX - j] == 0) {
							if (!isInRangeOfBomb(mapX - j, mapY + i)) {
								validDirection[Player::Down] = true;
								break;
							}
							else if (i + j < 5) {
								if (mapY + i + 1 < mapH && map[mapY + i + 1][mapX - j] == 0 && !isInRangeOfBomb(mapX - j, mapY + i + 1)) {
									validDirection[Player::Down] = true;
									break;
								}
								if (map[mapY + i - 1][mapX - j] == 0 && !isInRangeOfBomb(mapX - j, mapY + i - 1)) {
									validDirection[Player::Down] = true;
									break;
								}
							}
						}
						else break;
					}
					for (int j = 1; i + j < 6 && !validDirection[Player::Down] && mapX + j < mapW; ++j) {
						if (map[mapY + i][mapX + j] == 0) {
							if (!isInRangeOfBomb(mapX + j, mapY - i)) {
								validDirection[Player::Down] = true;
								break;
							}
							else if (i + j < 5) {
								if (mapY + i + 1 < mapH && map[mapY + i + 1][mapX + j] == 0 && !isInRangeOfBomb(mapX + j, mapY + i + 1)) {
									validDirection[Player::Down] = true;
									break;
								}
								if (map[mapY + i - 1][mapX + j] == 0 && !isInRangeOfBomb(mapX + j, mapY + i + 1)) {
									validDirection[Player::Down] = true;
									break;
								}
							}
						}
						else break;
					}
				}
			}
			else {
				break;
			}

		}
		//Left
		for (int i = 1; i < 6 && !validDirection[Player::Left] && mapX - i > 0; ++i) {
			if (map[mapY][mapX - i] == 0) {
				if (!isInRangeOfBomb(mapX - i, mapY)) {
					validDirection[Player::Left] = true;
					break;
				}
				else {
					for (int j = 1; i + j < 6 && !validDirection[Player::Left] && mapY - j >0; ++j) {
						if (map[mapY - j][mapX - i] == 0) {
							if (!isInRangeOfBomb(mapX - i, mapY - j)) {
								validDirection[Player::Left] = true;
								break;
							}
							else if (i + j < 5) {
								if (mapX - i - 1 > 0 && map[mapY - j][mapX - i - 1] == 0 && !isInRangeOfBomb(mapX - i - 1, mapY - j)) {
									validDirection[Player::Left] = true;
									break;
								}
								if (map[mapY - j][mapX - i + 1] == 0 && !isInRangeOfBomb(mapX - i + 1, mapY - j)) {
									validDirection[Player::Left] = true;
									break;
								}
							}
						}
						else {
							break;
						}
					}
					for (int j = 1; i + j < 6 && !validDirection[Player::Left] && mapY + j < mapH; ++j) {
						if (map[mapY + j][mapX - i] == 0) {
							if (!isInRangeOfBomb(mapX - i, mapY + j)) {
								validDirection[Player::Left] = true;
								break;
							}
							else if (i + j < 5) {
								if (mapX - i - 1 > 0 && map[mapY + j][mapX - i - 1] == 0 && !isInRangeOfBomb(mapX - i - 1, mapY + j)) {
									validDirection[Player::Left] = true;
									break;
								}
								if (map[mapY + j][mapX - i + 1] == 0 && !isInRangeOfBomb(mapX - i + 1, mapY + j)) {
									validDirection[Player::Left] = true;
									break;
								}
							}
						}
						else {
							break;
						}
					}
				}
			}
			else {
				break;
			}
		}
		//Right
		for (int i = 1; i < 6 && !validDirection[Player::Right] && mapX + i < mapW; ++i) {
			if (map[mapY][mapX + i] == 0) {
				if (!isInRangeOfBomb(mapX + i, mapY)) {
					validDirection[Player::Right] = true;
					break;
				}
				else {
					for (int j = 1; i + j < 6 && !validDirection[Player::Left] && mapY - j > 0; ++j) {
						if (map[mapY - j][mapX + i] == 0) {
							if (!isInRangeOfBomb(mapX + i, mapY - j)) {
								validDirection[Player::Right] = true;
								break;
							}
							else if (i + j < 5) {
								if (map[mapY - j][mapX + i - 1] == 0 && !isInRangeOfBomb(mapX + i - 1, mapY - j)) {
									validDirection[Player::Right] = true;
									break;
								}
								if (mapX + i + 1 < mapW  && map[mapY - j][mapX + i + 1] == 0 && !isInRangeOfBomb(mapX + i + 1, mapY - j)) {
									validDirection[Player::Right] = true;
									break;
								}
							}
						}
						else {
							break;
						}
					}
					for (int j = 1; i + j < 6 && !validDirection[Player::Left] && mapY + j < mapH; ++j) {
						if (map[mapY + j][mapX + i] == 0) {
							if (!isInRangeOfBomb(mapX + i, mapY + j)) {
								validDirection[Player::Right] = true;
								break;
							}
							else if (i + j < 5) {
								if (map[mapY + j][mapX + i - 1] == 0 && !isInRangeOfBomb(mapX + i - 1, mapY + j)) {
									validDirection[Player::Right] = true;
									break;
								}
								if (mapX + i + 1 < mapW && map[mapY + j][mapX + i + 1] == 0 && !isInRangeOfBomb(mapX + i + 1, mapY + j)) {
									validDirection[Player::Right] = true;
									break;
								}
							}
						}
						else {
							break;
						}
					}
				}
			}
			else {
				break;
			}
		}
		//End of "pathfinding"
		if (validDirection[0] || validDirection[1] || validDirection[2] || validDirection[3]) {
			int dir = std::rand() % 4;
			while (!validDirection[dir]) {
				++dir;
				dir %= 4;
			}
			ai->Update(Timer::getDTime(), checkCollision(ai), ai->setDirection(dir), ai->setSpeed(2));
		}
		else {
			ai->state = AIplayer::NoEscape;
			ai->Update(Timer::getDTime(), -1.0f, false, ai->setSpeed(0));
		}

	}
	else {
		ai->state = AIplayer::Default;
		if (map[mapY - 1][mapX] == 0 && !isInRangeOfBomb(mapX, mapY - 1)) {
			validDirection[Player::Up] = true;
		}
		if (map[mapY + 1][mapX] == 0 && !isInRangeOfBomb(mapX, mapY + 1)) {
			validDirection[Player::Down] = true;
		}
		if (map[mapY][mapX - 1] == 0 && !isInRangeOfBomb(mapX - 1, mapY)) {
			validDirection[Player::Left] = true;
		}
		if (map[mapY][mapX + 1] == 0 && !isInRangeOfBomb(mapX + 1, mapY)) {
			validDirection[Player::Right] = true;
		}
		if (map[mapY - 1][mapX] == 2 || map[mapY - 1][mapX] == 3 || map[mapY + 1][mapX] == 2 || map[mapY + 1][mapX] == 3 || map[mapY][mapX - 1] == 2 || map[mapY][mapX - 1] == 3 || map[mapY][mapX + 1] == 2 || map[mapY][mapX + 1] == 3) {
			placeBomb(ai);
		}
		Player* dummy = checkForPlayer(mapX, mapY - 1);
		if (dummy != NULL && dummy != ai) {
			placeBomb(ai);
		}
		dummy = checkForPlayer(mapX, mapY + 1);
		if (dummy != NULL && dummy != ai) {
			placeBomb(ai);
		}
		dummy = checkForPlayer(mapX - 1, mapY);
		if (dummy != NULL && dummy != ai) {
			placeBomb(ai);
		}
		dummy = checkForPlayer(mapX + 1, mapY);
		if (dummy != NULL && dummy != ai) {
			placeBomb(ai);
		}
		if (validDirection[0] || validDirection[1] || validDirection[2] || validDirection[3]) {
			int dir = std::rand() % 4;
			while (!validDirection[dir]) {
				++dir;
				dir %= 4;
			}
			ai->Update(Timer::getDTime(), checkCollision(ai), ai->setDirection(dir), ai->setSpeed(2));
		}
		else {
			ai->Update(Timer::getDTime(), -1.0f, false, ai->setSpeed(0));
		}
	}
}

bool Level::isInRangeOfBomb(int x, int y) {

	//Bomb range is supposed to have a range from 1 to 3, so we check 4 tiles in every direction
	if (map[y][x] == 4) {
		for (int j = 0; j < 4; ++j) {
			if (bombs[j]!=NULL && (int)bombs[j]->getHitbox().x/16 == x && (int)bombs[j]->getHitbox().y/16 == y) {
				return true;
			}
		}
		//return true;
	}
	for (int i = 1; i < 5 && y - i > 0; ++i) {
		if (map[y - i][x] == 1 || map[y - i][x] == 2 || map[y - i][x] == 3) {
			break;
		}
		else if (map[y - i][x] == 4) {
			for (int j = 0; j < 4; ++j) {
				if (bombs[j]!= NULL && (int)bombs[j]->getHitbox().x/16 == x && (int)bombs[j]->getHitbox().y/16 == y - i && bombs[j]->getRange() >= i) {
					return true;
				}
			}
			//return true;
		}
	}
	for (int i = 1; i < 5 && y + i < mapH; ++i) {
		if (map[y + i][x] == 1 || map[y + i][x] == 2 || map[y + i][x] == 3) {
			break;
		}
		else if (map[y + i][x] == 4) {
			for (int j = 0; j < 4; ++j) {
				if (bombs[j]!=NULL && (int)bombs[j]->getHitbox().x/16 == x && (int)bombs[j]->getHitbox().y/16 == y + i && bombs[j]->getRange() >= i) {
					return true;
				}
			}
			//return true;
		}
	}
	for (int i = 1; i < 5 && x - i > 0; ++i) {
		if (map[y][x - i] == 1 || map[y][x - i] == 2 || map[y][x - i] == 3) {
			break;
		}
		else if (map[y][x - i] == 4) {
			for (int j = 0; j < 4; ++j) {
				if (bombs[j]!=NULL && (int)bombs[j]->getHitbox().x/16 == x - i && (int)bombs[j]->getHitbox().y/16 == y && bombs[j]->getRange() >= i) {
					return true;
				}
			}
			//return true;
		}
	}
	for (int i = 1; i < 5 && x + i < mapW; ++i) {
		if (map[y][x + i] == 1 || map[y][x + i] == 2 || map[y][x + i] == 3) {
			break;
		}
		else if (map[y][x + i] == 4) {
			for (int j = 0; j < 4; ++j) {
				if (bombs[j]!=NULL && (int)bombs[j]->getHitbox().x/16 == x + i && (int)bombs[j]->getHitbox().y/16 == y && bombs[j]->getRange() >= i) {
					return true;
				}
			}
			//return true;
		}
	}
	return false;

}