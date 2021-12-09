#pragma once

#include "TextureManager.h"
#include "SoundManager.h"

class Level{
public:
	int mapH, mapW;
	int** map;
	int* objectCount;
	enum Objects {
		BlankTile,
		IndestructibleTile,
		DestructibleObjectStatic,
		DestructibleObjectAnimated,
		ConveyorBelt
	};
	int specialMechanics;
	enum Mechanics {
		None,
		ConveyorBelts,
		Ice
	};

	Level();
	~Level();
};

