#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Menu.h"



class Game {
private:
	SDL_Window* gameWindow;
	bool quitFlag;
public:
	MainMenu* MainM;
	Game();
	~Game();
	bool getQuitFlag(void);
	void setQuitFlag(void);
	void Init(void);
	void Quit(void);
};