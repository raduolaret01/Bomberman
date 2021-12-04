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

		}
		if (G.getQuitFlag())
			isRunning = false;
	}

	G.Quit();
	return 0;
}