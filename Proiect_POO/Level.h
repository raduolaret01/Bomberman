#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Obstacle.h"
#include "Player.h"
#include "Bomb.h"

class Level{
public:
	SDL_Rect MapTexture;
	SDL_Point mapTextureOffset;
	int mapTextureW, mapTextureH;
	int mapH, mapW;
	int** map;
	int* objectCount;
	GameObj*** objTable;
	Bomb** bombs;
	enum Objects {
		BlankTile,
		SolidObject,
		ObstacleStatic,
		ObstacleAnim,
		BombT,
		Conveyor
	};
	int specialMechanics;
	enum Mechanics {
		None,
		ConveyorBelts,
		Ice
	};
	int AIno;
	Player* Player1, * Player2, ** AI;

	Level();
	~Level();
	
	void Init(void);
	int Show(void);


	float checkCollision(Player*);
	void placeBomb(Player*);
	void explode(Bomb*);
	void deleteBomb(Bomb*);
	int victoryCheck(void);
	Player* checkForPlayer(int, int);

	void deleteObj(int, int);
};

