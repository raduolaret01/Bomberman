#pragma once

#include <fstream>
#include "TextureManager.h"
#include "SoundManager.h"
#include "Level.h"

class LevelLoader {
private:
	std::ifstream levelFile;

public:
	bool levelLoaded;
	enum Levels {
		Default,
		sp_1,
		sp_2,
		sp_3,
		mp_coop,
		mp_vs
	};
	LevelLoader();
	~LevelLoader();

	bool loadLevel(Level*,int);
	void unloadLevel(Level*);
};

