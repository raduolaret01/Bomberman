#pragma once
#include "TextureManager.h"
#include "SoundManager.h"
#include "Menu.h"

//todo:
//errorflag + set/get ca static pentru a putea avea
//buton de exit game


class Game {
private:
	SDL_Window* gameWindow;
	static bool quitFlag;
public:
	Game();
	~Game();
	bool getQuitFlag(void);
	static void setQuitFlag(void);
	static void clearQuitFlag(void);
	void Init(void);
	void Quit(void);
};