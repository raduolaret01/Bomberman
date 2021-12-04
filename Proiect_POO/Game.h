#pragma once
#include "TextureManager.h"
#include "SoundManager.h"


class Game {
private:
	SDL_Window* gameWindow;
	bool errorFlag;
public:
	Game();
	~Game();
	bool getErrorFlag(void);
	void setErrorFlag(bool);
	void Init(void);
	void Quit(void);
};