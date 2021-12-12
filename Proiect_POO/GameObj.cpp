#include "GameObj.h"

AnimationState::AnimationState() {
	secondsPerFrame = NULL;
}

AnimationState::~AnimationState() {
	if (secondsPerFrame) {
		delete[] secondsPerFrame;
		secondsPerFrame = NULL;
	}
}

//bool AnimationState::operator==(const AnimationState& target) {
//	return (textureArea.x == target.textureArea.x && textureArea.y == target.textureArea.y && textureArea.w == target.textureArea.w && textureArea.h == target.textureArea.h && numberOfFrames == target.numberOfFrames && frameOffset == target.frameOffset && secondsPerFrame == target.secondsPerFrame);
//}
//
//AnimationState& AnimationState::operator=(const AnimationState& source) {
//
//}

AnimationState* GameObj::animationStates = NULL;
int GameObj::numberOfAnimStates = 0;

GameObj::GameObj() {
	hitbox = { 0,0,0,0 };
	currentAnimState = NULL;
	currentAnimFrame = 0;
}

GameObj::GameObj(const SDL_Point coords, const int w, const int h) {
	hitbox.x = coords.x;
	hitbox.y = coords.y;
	hitbox.w = w;
	hitbox.h = h;
	currentAnimState = NULL;
	currentAnimFrame = 0;
}

GameObj::~GameObj() {

}

bool GameObj::loadAnimationStates(std::ifstream* input) {
	*input >> numberOfAnimStates;
	animationStates = new AnimationState[numberOfAnimStates];
	for (int i = 0; i < numberOfAnimStates; ++i) {
		*input >> animationStates[i].textureArea.x >> animationStates[i].textureArea.y >> animationStates[i].textureArea.w >> animationStates[i].textureArea.h;
		*input >> animationStates[i].numberOfFrames >> animationStates[i].frameOffset;
		animationStates[i].secondsPerFrame = new int[animationStates[i].numberOfFrames];
		for (int j = 0; j < animationStates[i].numberOfFrames; ++i) {
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
