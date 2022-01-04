#include "Bomb.h"

AnimationState* Bomb::animationStates = NULL;
int Bomb::numberOfAnimStates = 2;

Bomb::Bomb() : SolidObj() {
	tick = 0;
	range = 1;
	owner = NULL;
	currentAnimState = animationStates;
	blast = NULL;
}

Bomb::Bomb(const SDL_Point coords, const int w, const int h, Player* placer) : SolidObj(coords, w, h) {
	tick = 0;
	this->range = placer->range;
	owner = placer;
	currentAnimState = animationStates;
	blast = NULL;
}

Bomb::~Bomb() {
	if (blast != NULL) {
		delete[] blast;
		blast = NULL;
	}
}

int Bomb::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Bomb::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Bomb::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Bomb::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime / 60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		if (isOnLastAnimFrame() && currentAnimState == animationStates + Explosion) {
			return -1;
		}
		
		if (currentAnimState->numberOfFrames > 1 && isOnLastAnimFrame()) {
			currentAnimFrame = 0;
			currentFrameTime = 0;
		}
		else {
			++tick;
			++currentAnimFrame;
			currentFrameTime = 0;
		}
	}
	if (currentAnimState == animationStates + Explosion) {
		for (int i = 0; i < blastCount; ++i) {
			if (blast) {
				blast[i]->Show(Offset);
			}
		}
	}
	if (tick == 6 && currentAnimState == animationStates) {
		return 1;
	}
	SDL_Rect TempSrc;
	if (currentAnimState == animationStates + Explosion) {
		TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y ,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	}
	else {
		TempSrc = { currentAnimState->textureArea.x + range * 17 ,currentAnimState->textureArea.y + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	}
	SDL_Rect TempDest = { (int)(hitbox.x * 2) + Offset.x,(int)(hitbox.y * 2) + Offset.y,hitbox.w * 2,hitbox.h * 2 };
	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::BombTileSet], &TempSrc, &TempDest);

	return 0;
}

int Bomb::getRange() {
	return range;
}

void Bomb::setBlast(Blast** blastArr,int count) {
	blast = blastArr;
	blastCount = count;
}

bool Bomb::loadAnimationStates(std::ifstream* input) {
	animationStates = new AnimationState[numberOfAnimStates];
	for (int i = 0; i < numberOfAnimStates; ++i) {
		*input >> animationStates[i].textureArea.x >> animationStates[i].textureArea.y >> animationStates[i].textureArea.w >> animationStates[i].textureArea.h;
		*input >> animationStates[i].numberOfFrames >> animationStates[i].frameOffset;
		animationStates[i].secondsPerFrame = new int[animationStates[i].numberOfFrames];
		for (int j = 0; j < animationStates[i].numberOfFrames; ++j) {
			*input >> animationStates[i].secondsPerFrame[j];
		}
	}
	return true;
}

void Bomb::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}


AnimationState* Blast::animationStates = NULL;
int Blast::numberOfAnimStates = 4;

Blast::Blast() : GameObj() {
	direction = 0;
}

Blast::Blast(const SDL_Point coords, const int w, const int h,const int direction) : GameObj(coords, w, h) {
	this->direction = direction;
}

Blast::~Blast() {

}

int Blast::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Blast::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Blast::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Blast::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime / 60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		if (isOnLastAnimFrame()) {
			//return -1;
		}
		else {
			++currentAnimFrame;
			currentFrameTime = 0;
		}
	}
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (direction == Right) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (direction == Down) {
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y ,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { (int)(hitbox.x * 2) + Offset.x,(int)(hitbox.y * 2) + Offset.y,hitbox.w * 2,hitbox.h * 2 };
	SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::BombTileSet], &TempSrc, &TempDest, 0, NULL, flip);

	return 0;
}

bool Blast::loadAnimationStates(std::ifstream* input) {
	animationStates = new AnimationState[numberOfAnimStates];
	for (int i = 0; i < numberOfAnimStates; ++i) {
		*input >> animationStates[i].textureArea.x >> animationStates[i].textureArea.y >> animationStates[i].textureArea.w >> animationStates[i].textureArea.h;
		*input >> animationStates[i].numberOfFrames >> animationStates[i].frameOffset;
		animationStates[i].secondsPerFrame = new int[animationStates[i].numberOfFrames];
		for (int j = 0; j < animationStates[i].numberOfFrames; ++j) {
			*input >> animationStates[i].secondsPerFrame[j];
		}
	}
	return true;
}

void Blast::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}
