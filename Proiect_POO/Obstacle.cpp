#include "Obstacle.h"

int Obstacle::numberOfAnimStates = 2;

Obstacle::Obstacle() :SolidObj() {

}

Obstacle::Obstacle(const SDL_Point coords, const int w, const int h) : SolidObj(coords, w, h) {

}

Obstacle::~Obstacle() {
	unloadAnimationStates();
}

int Obstacle::getAnimState() {
	return 0;
}

void Obstacle::setAnimState(int x) {
	return;
}

bool Obstacle::isOnLastAnimFrame() {
	return false;
}

int Obstacle::Show(SDL_Point) {
	return 0;
}

AnimationState* Obstacle1::animationStates = NULL;

Obstacle1::Obstacle1() :Obstacle() {
	currentAnimState = animationStates;
}

Obstacle1::Obstacle1(const SDL_Point coords, const int w, const int h) : Obstacle(coords, w, h) {
	currentAnimState = animationStates;
}

Obstacle1::~Obstacle1() {
	unloadAnimationStates();
}

int Obstacle1::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Obstacle1::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Obstacle1::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Obstacle1::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime/60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		if (isOnLastAnimFrame() && currentAnimState == animationStates + BlowingUp) {
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
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { hitbox.x * 2 + Offset.x,hitbox.y * 2 + Offset.y,hitbox.w * 2,hitbox.h * 2 };
	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::LevelTileSet], &TempSrc, &TempDest);

	return 0;
}

bool Obstacle1::loadAnimationStates(std::ifstream* input) {
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

void Obstacle1::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}

AnimationState* Obstacle2::animationStates = NULL;

Obstacle2::Obstacle2() :Obstacle() {
	currentAnimState = animationStates;
}

Obstacle2::Obstacle2(const SDL_Point coords, const int w, const int h) : Obstacle(coords, w, h) {
	currentAnimState = animationStates;
}

Obstacle2::~Obstacle2() {
	unloadAnimationStates();
}

int Obstacle2::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Obstacle2::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Obstacle2::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Obstacle2::Show(SDL_Point Offset) {
	
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime/60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		if (isOnLastAnimFrame() && currentAnimState == animationStates + BlowingUp) {
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
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { hitbox.x * 2 + Offset.x,hitbox.y * 2 + Offset.y,hitbox.w * 2,hitbox.h * 2 };
	SDL_RenderCopy(TextureManager::Renderer, TextureManager::Texture[TextureManager::LevelTileSet], &TempSrc, &TempDest);

	return 0;
}

bool Obstacle2::loadAnimationStates(std::ifstream* input) {
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

void Obstacle2::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}