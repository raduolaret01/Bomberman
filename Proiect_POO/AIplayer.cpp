#include "AIplayer.h"

int AIplayer::numberOfAnimStates = 8;
AnimationState* AIplayer::animationStates = NULL;

AIplayer::AIplayer() : Player() {
	state = Default;
	reactionTime = 0;
}

AIplayer::AIplayer(const SDL_Point coord, const int w, const int h) : Player(coord, w, h) {
	state = Default;
	reactionTime = 0;
}

AIplayer::~AIplayer() {

}

int AIplayer::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void AIplayer::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool AIplayer::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int AIplayer::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime / 60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {

		if (currentAnimState == animationStates + Defeat && isOnLastAnimFrame()) {
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
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (direction == Left) {
		flip = SDL_FLIP_HORIZONTAL;
	}

	SDL_Point spriteOffset = { (currentAnimState->textureArea.w - 12) / 2,currentAnimState->textureArea.h - 12 };
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { (int)((hitbox.x - spriteOffset.x) * 2 + Offset.x),(int)((hitbox.y - spriteOffset.y) * 2 + Offset.y),currentAnimState->textureArea.w * 2,currentAnimState->textureArea.h * 2 };
	SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::PlayerAI], &TempSrc, &TempDest, 0, NULL, flip);

	//SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::Player1], &TempSrc, &hitbox, 0, NULL, flip);
	return 0;
}

bool AIplayer::loadAnimationStates(std::ifstream* input) {
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

void AIplayer::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}

