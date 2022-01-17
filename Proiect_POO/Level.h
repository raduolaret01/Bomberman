#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Obstacle.h"
#include "AIplayer.h"
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
	int specialMechanics;	//unused
	enum Mechanics {
		None,
		ConveyorBelts,
		Ice
	};
	int AIno,playerNo;
	Player* Player1, * Player2;
	AIplayer** AI;

	Level();
	~Level();
	
	void Init(void);
	int Show(void);


	float checkCollision(Player*);
	void placeBomb(Player*);
	void explode(Bomb*);
	void deleteBomb(void);
	int victoryCheck(void);
	Player* checkForPlayer(int, int);
	void AIDecisionMaking(AIplayer*);
	bool isInRangeOfBomb(int, int);

	void deleteObj(int, int);
};

