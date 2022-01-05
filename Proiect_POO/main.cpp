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
			Menu::clearFlags();
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
				Timer::setDTime();
			}
			if (G.Menus[Game::Main]->getLoadLevelFlag()) {
				if (G.level == NULL) {
					G.level = new Level;
				}
				G.levelLoader.levelLoaded = G.levelLoader.loadLevel(G.level, G.Menus[Game::Main]->getLoadLevelFlag());
				G.level->Init();

				int pauseMenuTimer = 0;
				while (G.levelLoader.levelLoaded && isRunning) {
					SDL_PollEvent(&e);
					const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
					if (currentKeyStates[SDL_SCANCODE_ESCAPE] && pauseMenuTimer >= 250) {
						Menu::setPauseMenuFlag();
						SDL_Texture* TempTexture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 640, 480);
						if (TempTexture == NULL) {
							printf("Unable to render blank background texture! SDL Error: %s\n", SDL_GetError());
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
						pauseMenuTimer = 0;
						while (G.Menus[Game::Pause]->getPauseMenuFlag() && !(G.Menus[Game::Main]->getQuitFlag()) && isRunning) {
							while (SDL_PollEvent(&e) != 0) {
								if (e.type == SDL_QUIT)
									isRunning = false;
								for (int i = 0; i < 3; ++i) {
									G.Menus[Game::Pause]->ButtonArray[i]->handleEvent(&e);
								}
								const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
								if (currentKeyStates[SDL_SCANCODE_ESCAPE] && pauseMenuTimer >= 500) {
									Menu::clearPauseMenuFlag();
								}
							}
							SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
							SDL_RenderClear(TextureManager::Renderer);
							G.Menus[Game::Pause]->Show();
							pauseMenuTimer += Timer::getDTime();
							Timer::setDTime();
						}
						pauseMenuTimer = 0;
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
					//if (currentKeyStates[SDL_SCANCODE_SPACE]) {
					//	if(G.level)
					//		G.level->objTable[2][0]->setAnimState(1);
					//}
					else if (G.level != NULL) {
						if (G.level->Player1 != NULL && !G.level->Player1->isDead) {
							if (currentKeyStates[SDL_SCANCODE_W]) {
								G.level->Player1->Update(Timer::getDTime(), G.level->checkCollision(G.level->Player1), G.level->Player1->setDirection(Player::Up), G.level->Player1->setSpeed(2));
							}
							if (currentKeyStates[SDL_SCANCODE_D]) {
								G.level->Player1->Update(Timer::getDTime(), G.level->checkCollision(G.level->Player1), G.level->Player1->setDirection(Player::Right), G.level->Player1->setSpeed(2));
							}
							if (currentKeyStates[SDL_SCANCODE_S]) {
								G.level->Player1->Update(Timer::getDTime(), G.level->checkCollision(G.level->Player1), G.level->Player1->setDirection(Player::Down), G.level->Player1->setSpeed(2));
							}
							if (currentKeyStates[SDL_SCANCODE_A]) {
								G.level->Player1->Update(Timer::getDTime(), G.level->checkCollision(G.level->Player1), G.level->Player1->setDirection(Player::Left), G.level->Player1->setSpeed(2));
							}
							if (!(currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_D])) {
								G.level->Player1->Update(Timer::getDTime(), -1.0f, false, G.level->Player1->setSpeed(0));
							}

							if (currentKeyStates[SDL_SCANCODE_SPACE]) {
								G.level->placeBomb(G.level->Player1);
							}
						}

						if (e.type == SDL_QUIT) {
							isRunning = false;
						}

						SDL_SetRenderDrawColor(TextureManager::Renderer, 0x00, 0x00, 0x00, 0xFF);
						SDL_RenderClear(TextureManager::Renderer);
						int vCheck = G.level->Show();

						if (vCheck == -1) {
							Menu::setPauseMenuFlag();
							SDL_Texture* TempTexture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 640, 480);
							if (TempTexture == NULL) {
								printf("Unable to render blank background texture! SDL Error: %s\n", SDL_GetError());
							}
							else {
								SDL_SetRenderTarget(TextureManager::Renderer, TempTexture);
								G.level->Show();
								SDL_SetRenderTarget(TextureManager::Renderer, NULL);
							}
							if (G.Menus[Game::GameOverScreen]->Background) {
								SDL_DestroyTexture(G.Menus[Game::GameOverScreen]->Background);
							}
							G.Menus[Game::GameOverScreen]->Background = TempTexture;
							G.Menus[Game::GameOverScreen]->Show();
							while (G.Menus[Game::GameOverScreen]->getPauseMenuFlag() && !(G.Menus[Game::Main]->getQuitFlag()) && isRunning) {
								while (SDL_PollEvent(&e) != 0) {
									if (e.type == SDL_QUIT)
										isRunning = false;
									for (int i = 0; i < 3; ++i) {
										G.Menus[Game::GameOverScreen]->ButtonArray[i]->handleEvent(&e);
									}
								}
								SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
								SDL_RenderClear(TextureManager::Renderer);
								G.Menus[Game::GameOverScreen]->Show();
								Timer::setDTime();
							}
							if (G.Menus[Game::GameOverScreen]->getReloadFlag()) {
								G.levelLoader.unloadLevel(G.level);
								G.level = NULL;
								Menu::clearFlags();
								Menu::setMainMenuFlag();
								G.Menus[Game::Main]->Show();
							}
							else if (G.Menus[Game::GameOverScreen]->getQuitFlag()) {
								G.levelLoader.unloadLevel(G.level);
								G.level = NULL;
							}
							else if (G.Menus[Game::GameOverScreen]->getReplayLevelFlag()) {
								G.levelLoader.unloadLevel(G.level);
								G.level = new Level;
								G.levelLoader.levelLoaded = G.levelLoader.loadLevel(G.level, G.Menus[Game::Main]->getLoadLevelFlag());
								G.level->Init();
							}
						}
						
					}
					pauseMenuTimer += Timer::getDTime();
					Timer::setDTime();
				}
			}
		}
		if (G.getQuitFlag() || G.Menus[Game::Main]->getQuitFlag()) {
			isRunning = false;
		}
		Timer::setDTime();
	}

	return 0;
}