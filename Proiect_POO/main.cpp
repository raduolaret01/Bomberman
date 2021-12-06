#include "Game.h"

int main() {
	Game G;
	G.Init();
	bool isRunning = !G.getQuitFlag();
	SDL_Event e;

	while (isRunning) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				isRunning = false;
			while (G.MainM->getMainMenuFlag() && !(G.MainM->getQuitFlag()) && isRunning) {
				if (e.type == SDL_QUIT)
					isRunning = false;
				while (SDL_PollEvent(&e) != 0) {
					for (int i = 0; i < 3; ++i) {
						G.MainM->ButtonArray[i]->handleEvent(&e);
					}
				}
			}
		}
		if (G.getQuitFlag() || G.MainM->getQuitFlag());
			isRunning = false;
	}

	return 0;
}