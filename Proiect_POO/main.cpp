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
			while (G.Menus[Game::Main]->getMainMenuFlag() && !(G.Menus[Game::Main]->getQuitFlag()) && isRunning) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						isRunning = false;
					for (int i = 0; i < 6; ++i) {
						G.Menus[Game::Main]->ButtonArray[i]->handleEvent(&e);
					}
				}
				SDL_SetRenderDrawColor(TextureManager::Renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(TextureManager::Renderer);
				G.Menus[Game::Main]->Show();
			}
			if (G.Menus[Game::Main]->getLoadLevelFlag()) {

				if (G.level == NULL) {
					G.level = new Level;
				}
				G.levelLoader.levelLoaded = G.levelLoader.loadLevel(G.level, G.Menus[Game::Main]->getLoadLevelFlag());
				Menu::clearFlags();

				while (G.levelLoader.levelLoaded && isRunning) {
					while (SDL_PollEvent(&e) != 0) {
						const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
						if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {

							Menu::setPauseMenuFlag(); SDL_Texture* TempTexture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 600, 400);
							if (TempTexture == NULL) {
								printf("Unable to render blank background texture! SDL_ttf Error: %s\n", TTF_GetError());
							}
							else {
								SDL_SetRenderTarget(TextureManager::Renderer, TempTexture);
								G.level->Show();
								SDL_SetRenderTarget(TextureManager::Renderer, NULL);
							}
							if (G.Menus[Game::Pause]->Background) {
								SDL_DestroyTexture(G.Menus[Game::Pause]->Background);
							}
							G.Menus[Game::Pause]->Background = TempTexture;
							G.Menus[Game::Pause]->Show();

							while (G.Menus[Game::Pause]->getPauseMenuFlag() && !(G.Menus[Game::Main]->getQuitFlag()) && isRunning) {
								while (SDL_PollEvent(&e) != 0) {
									if (e.type == SDL_QUIT)
										isRunning = false;
									for (int i = 0; i < 3; ++i) {
										G.Menus[Game::Pause]->ButtonArray[i]->handleEvent(&e);
									}
									const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
									if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
										Menu::clearPauseMenuFlag();
									}
								}
								SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
								SDL_RenderClear(TextureManager::Renderer);
								G.Menus[Game::Pause]->Show();
							}
							if (G.Menus[Game::Pause]->getReloadFlag()) {
								G.levelLoader.unloadLevel(G.level);
								G.level = NULL;
								Menu::clearFlags();
								Menu::setMainMenuFlag();
								G.Menus[Game::Main]->Show();
							}
							else if (G.Menus[Game::Pause]->getQuitFlag()) {
								G.levelLoader.unloadLevel(G.level);
								G.level = NULL;
							}

						}
						if (e.type == SDL_QUIT)
							isRunning = false;
					}
					SDL_SetRenderDrawColor(TextureManager::Renderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(TextureManager::Renderer);
					G.level->Show();
				}
			}
		}
		if (G.getQuitFlag() || G.Menus[Game::Main]->getQuitFlag()) {
			isRunning = false;
		}
	}

	return 0;
}