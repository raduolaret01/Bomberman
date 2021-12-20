#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Obstacle.h"
#include "Player.h"

class Level{
public:
	SDL_Rect MapTexture;
	SDL_Point mapTextureOffset;
	int mapTextureW, mapTextureH;
	int mapH, mapW;
	int** map;
	int* objectCount;
	GameObj*** objTable;
	enum Objects {
		BlankTile,
		SolidObject,
		ObstacleStatic,
		ObstacleAnim,
		Conveyor
	};
	int specialMechanics;
	enum Mechanics {
		None,
		ConveyorBelts,
		Ice
	};
	Player* Player1;

	Level();
	~Level();
	
	void Init(void);
	void Show(void);


	float checkCollision(Player*);
	void deleteObj(int, int);
};

