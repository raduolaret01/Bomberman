#include "Button.h"

Button::Button() {
	Texture = NULL;
	label = NULL;
	dim.x = dim.y = dim.w = dim.h = 0;
	functionality = NULL;
	soundFlag = false;
}

Button::Button(SDL_Rect dim, void(*function)(void), const char* label) {
	int len = (int)strlen(label) + 1;
	this->label = new char[len];
	strcpy_s(this->label, len, label);
	this->dim = dim;
	functionality = function;
	soundFlag = false;

	Texture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, dim.w, dim.h);
	if (Texture == NULL) {
		Logs::logF << "Unable to render blank button texture! SDL Error: " << SDL_GetError() << std::endl;
	}
	else {
		SDL_SetRenderTarget(TextureManager::Renderer, Texture);

		SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFF, 0xBC, 0x00, 0xFF);
		SDL_Rect temp = { 0,0,dim.w,dim.h };
		SDL_RenderFillRect(TextureManager::Renderer, &temp);

		SDL_SetRenderDrawColor(TextureManager::Renderer, 0xFB, 0xFF, 0x62, 0xFF);
		temp.x += 6;
		temp.y += 4;
		temp.w -= 6;
		temp.h -= 4;
		SDL_RenderFillRect(TextureManager::Renderer, &temp);

		SDL_Surface* textSurface = TTF_RenderText_Solid(TextureManager::Font[0], this->label, { 0,0,0 });
		if (textSurface == NULL) {
			Logs::logF << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
		}
		else {
			int tWidth, tHeight;
			//Create texture from surface pixels
			SDL_Texture* tTexture = SDL_CreateTextureFromSurface(TextureManager::Renderer, textSurface);
			if (tTexture == NULL) {
				Logs::logF << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
			}
			else {
				//Get image dimensions
				tWidth = textSurface->w;
				tHeight = textSurface->h;
				//Set text to render centered
				temp.x = (dim.w - tWidth) / 2;
				temp.y = (dim.h - tHeight) / 2;
				temp.w = tWidth;
				temp.h = tHeight;
				SDL_RenderCopy(TextureManager::Renderer, tTexture, NULL, &temp);
				SDL_RenderPresent(TextureManager::Renderer);
				//Get rid of old surface
				SDL_FreeSurface(textSurface);
				SDL_DestroyTexture(tTexture);
			}
		}
		SDL_SetRenderTarget(TextureManager::Renderer, NULL);
	}
}

Button::~Button() {
	if (Texture) {
		SDL_DestroyTexture(Texture);
		Texture = NULL;
	}
	if (label) {
		delete[] label;
		label = NULL;
	}
	functionality = NULL;
}

void Button::handleEvent(SDL_Event* e) {
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		//Check if mouse is in button
		bool inside = false;

		if (x > dim.x && x < dim.x + dim.w && y > dim.y && y < dim.y + dim.h) {
			inside = true;
		}

		//Mouse is inside button
		if (inside) {
			//Set mouse over sprite
			switch (e->type) {
			case SDL_MOUSEMOTION:
				if (SDL_SetTextureColorMod(Texture, 0x7F, 0x7F, 0x7F) < 0) {
					Logs::logF << "Button Color Modulation failed! SDL Error: " << SDL_GetError() << std::endl;
				}
				if (!soundFlag) {
					Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::MouseOver], 0);
					soundFlag = true;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				Mix_PlayChannel(-1, SoundManager::SFX[SoundManager::Click], 0);
				functionality();
				break;
			}
		}
		else {
			if (SDL_SetTextureColorMod(Texture, 0xFF, 0xFF, 0xFF) < 0) {
				Logs::logF << "Button Color Modulation failed! SDL Error: " << SDL_GetError() << std::endl;
			}
			soundFlag = false;
		}
	}
}

void Button::Show() {
	SDL_RenderCopy(TextureManager::Renderer, this->Texture, NULL, &dim);
}