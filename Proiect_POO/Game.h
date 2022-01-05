#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Menu.h"
#include "LevelLoader.h"



class Game {
private:
	SDL_Window* gameWindow;
	bool quitFlag;
public:
	Menu* Menus[4];
	enum menus {
		Main,
		Pause,
		GameOverScreen,
		VictoryScreen
	};
	LevelLoader levelLoader;
	Level* level;
	Game();
	~Game();
	bool getQuitFlag(void);
	void setQuitFlag(void);
	void Init(void);
	void Quit(void);
};