#include "Bomb.h"

AnimationState* Bomb::animationStates = NULL;
int Bomb::numberOfAnimStates = 2;

Bomb::Bomb() : SolidObj() {
	range = 1;
	owner = NULL;
	currentAnimState = animationStates;
}

Bomb::Bomb(const SDL_Point coords, const int w, const int h, Player* placer) : SolidObj(coords, w, h) {
	this->range = placer->range;
	owner = placer;
	currentAnimState = animationStates;
}

Bomb::~Bomb() {

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
			++currentAnimFrame;
			currentFrameTime = 0;
		}
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
