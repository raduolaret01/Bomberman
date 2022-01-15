#pragma once

#include <string.h>
#include "TextureManager.h"
#include "SoundManager.h"

class Button{
private:
	SDL_Texture* Texture;
	char* label;
	SDL_Rect dim;
	void (*functionality)(void);
	bool soundFlag;
public:
	Button();
	Button(SDL_Rect, void (*function)(void), const char*);
	~Button();

	void handleEvent(SDL_Event*);
	void Show(void);

};

