#include "Menu.h"

bool Menu::quitFlag = false;

Menu::Menu() {
	title = NULL;
	ButtonArray = NULL;
	isShown = false;
}

Menu::~Menu() {
	if (title) {
		delete[] title;
		title = NULL;
	}
}

bool Menu::getQuitFlag() {
	return quitFlag;
}

void Menu::setQuitFlag() {
	quitFlag = true;
}

void Menu::Hide() {
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(TextureManager::Renderer);
	isShown = false;
}

MainMenu::MainMenu() :Menu() {}

MainMenu::~MainMenu() {
	if (ButtonArray) {
		for (int i = 0; i < 6; ++i) {
			if (ButtonArray[i]) {
				delete ButtonArray[i];
				ButtonArray[i] = NULL;
			}
		}
		delete[] ButtonArray;
		ButtonArray = NULL;
	}
}

void MainMenu::Init() {
	title = new char[10];
	strcpy_s(title, 10, "Bomberman");

	ButtonArray = new Button*[6];
	ButtonArray[Quit] = new Button({ 200,310,200,60 }, &Menu::setQuitFlag, "Quit");
	ButtonArray[Load_1p_1] = new Button({ 150,110,100,60 }, NULL, "Level 1");
	ButtonArray[Load_1p_2] = new Button({ 250,110,100,60 }, NULL, "Level 2");
	ButtonArray[Load_1p_3] = new Button({ 350,110,100,60 }, NULL, "Level 3");
	ButtonArray[Load_2p_1] = new Button({ 150,170,150,60 }, NULL, "2 Player CO-OP");
	ButtonArray[Load_2p_2] = new Button({ 300,170,150,60 }, NULL, "2 Player Versus");

}

void MainMenu::Free() {
	this->~MainMenu();
}

void MainMenu::Show() {
	//Send background to renderer
	SDL_SetRenderDrawColor(TextureManager::Renderer, 0x11, 0x05, 0x80, 0xFF);
	SDL_RenderFillRect(TextureManager::Renderer, NULL);

	//Transform title text into a texture and send it to renderer
	SDL_Surface* textSurface = TTF_RenderText_Solid(TextureManager::Font[0], title, { 0xFF,0xFF,0xFF });
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		int tWidth, tHeight;
		//Create texture from surface pixels
		SDL_Texture* tTexture = SDL_CreateTextureFromSurface(TextureManager::Renderer, textSurface);
		if (tTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			//Get text image dimensions
			tWidth = textSurface->w;
			tHeight = textSurface->h;

			//Set text to render centered on x axis
			SDL_Rect temp;
			temp.x = (600 - tWidth) / 2;
			temp.y = 30;
			temp.w = tWidth;
			temp.h = tHeight;
			SDL_RenderCopy(TextureManager::Renderer, tTexture, NULL, &temp);
			//Get rid of old surface
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(tTexture);
		}
	}

	for (int i = 0; i < 6; ++i) {
		ButtonArray[i]->Show();
	}

	isShown = true;
	SDL_RenderPresent(TextureManager::Renderer);
}
