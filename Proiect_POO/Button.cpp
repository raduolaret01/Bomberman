#include "Button.h"

Button::Button() {
	Texture = NULL;
	label = NULL;
	dim.x = dim.y = dim.w = dim.h = 0;
	functionality = NULL;
}

Button::Button(SDL_Rect dim, void(*function)(void), const char* label) {
    int len = strlen(label)+1;
    this->label = new char[len];
	strcpy_s(this->label, len, label);
	this->dim = dim;
	functionality = function;

	Texture = SDL_CreateTexture(TextureManager::Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.w, dim.h);
    if (Texture == NULL) {
        printf("Unable to render blank button texture! SDL_ttf Error: %s\n", TTF_GetError());
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
                //Get image dimensions
                tWidth = textSurface->w;
                tHeight = textSurface->h;
            }
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
        SDL_SetRenderTarget(TextureManager::Renderer, NULL);
        SDL_RenderCopy(TextureManager::Renderer, this->Texture, NULL, &dim);
        SDL_RenderPresent(TextureManager::Renderer);
    }
}

Button::~Button() {
    if (Texture)
        SDL_DestroyTexture(Texture);
    Texture = NULL;
    if (label)
        delete[] label;
    label = NULL;
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
        bool inside = true;

        //Mouse is left of the button
        if (x < dim.x) {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > dim.x + dim.w) {
            inside = false;
        }
        //Mouse above the button
        else if (y < dim.y) {
            inside = false;
        }
        //Mouse below the button
        else if (y > dim.y + dim.h) {
            inside = false;
        }
        //Mouse is inside button
        if(inside) {
            //Set mouse over sprite
            switch (e->type) {
            /*case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;*/

            case SDL_MOUSEBUTTONUP:
                functionality();
                break;
            }
        }
    }
}