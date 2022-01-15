#include "GameObj.h"

AnimationState::AnimationState() {
	frameOffset = 0;
	numberOfFrames = 0;
	textureArea = { 0,0,0,0 };
	secondsPerFrame = NULL;
}

AnimationState::~AnimationState() {
	if (secondsPerFrame) {
		delete[] secondsPerFrame;
		secondsPerFrame = NULL;
	}
}

AnimationState* GameObj::animationStates = NULL;
int GameObj::numberOfAnimStates = 0;

GameObj::GameObj() {
	hitbox = { 0.0f,0.0f,0,0 };
	currentAnimState = NULL;
	currentAnimFrame = 0;
	currentFrameTime = 0;
}

GameObj::GameObj(const SDL_Point coords, const int w, const int h) {
	hitbox.x = (float)coords.x;
	hitbox.y = (float)coords.y;
	hitbox.w = w;
	hitbox.h = h;
	currentAnimState = NULL;
	currentAnimFrame = 0;
	currentFrameTime = 0;
}

GameObj::~GameObj() {
	unloadAnimationStates();
}

Hitbox GameObj::getHitbox() {
	return hitbox;
}

bool GameObj::loadAnimationStates(std::ifstream* input) {
	*input >> numberOfAnimStates;
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

void GameObj::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}

