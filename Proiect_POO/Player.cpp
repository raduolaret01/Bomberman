#include <cmath>
#include "Player.h"

int Player::numberOfAnimStates = 6;
AnimationState* Player::animationStates = NULL;

Player::Player() : SolidObj() {
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
}

Player::Player(const SDL_Point coord, const int w, const int h) : SolidObj(coord, w, h) {
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
}

Player::~Player() {

}

int Player::getAnimState() {
	return ((currentAnimState - animationStates) / sizeof(AnimationState));
}

void Player::setAnimState(int x) {
	currentAnimFrame = 0;
	currentAnimState = animationStates + x;
}

bool Player::isOnLastAnimFrame() {
	return currentAnimFrame == currentAnimState->numberOfFrames - 1;
}

int Player::Show(SDL_Point Offset) {
	currentFrameTime += Timer::getDTime();
	if (currentFrameTime / 60 >= currentAnimState->secondsPerFrame[currentAnimFrame]) {
		
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
	SDL_Rect TempSrc = { currentAnimState->textureArea.x + currentAnimFrame * currentAnimState->frameOffset,currentAnimState->textureArea.y,currentAnimState->textureArea.w,currentAnimState->textureArea.h };
	SDL_Rect TempDest = { (hitbox.x - 1) * 3 / 2 + Offset.x,(hitbox.y - 10) * 3 / 2 + Offset.y,currentAnimState->textureArea.w * 3 / 2,currentAnimState->textureArea.h * 3 / 2 };
	SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::Player1], &TempSrc, &TempDest,0,NULL,flip);
	return 0;
}

void Player::Update(int dTime, bool dirChanged, bool spdChanged) {
	if (speed == 2) {
		if (dirChanged||spdChanged) {
			switch (direction) {
			case Down:
				setAnimState(DownMove);
				break;
			case Right:
			case Left:
				setAnimState(SideMove);
				break;
			case Up:
				setAnimState(UpMove);
				break;
			}
		}
		switch (direction) {
		case Down:
			hitbox.y += speed * dTime/30;
			break;
		case Right:
			hitbox.x += speed * dTime/30;
			break;
		case Left:
			hitbox.x -= speed * dTime/30;
			break;
		case Up:
			hitbox.y -= speed * dTime/30;
			break;
		}
	}
	else {
		switch (direction) {
		case Down:
			setAnimState(DownStill);
			break;
		case Right:
		case Left:
			setAnimState(SideStill);
			break;
		case Up:
			setAnimState(UpStill);
			break;
		}
	}
	
}

bool Player::setDirection(int dir) {
	dir %= 4;
	if (direction != dir) {
		direction = dir;
		return true;
	}
	return false;
}

bool Player::setSpeed(int sp) {
	if (speed != sp) {
		speed = sp;
		return true;
	}
	return false;
}

//bool Player::loadAnimationStates() {
//	animationStates = new AnimationState[numberOfAnimStates];
//
//	animationStates[0].textureArea.x = 7;
//	animationStates[0].textureArea.y = 20;
//	animationStates[0].textureArea.w = 16;
//	animationStates[0].textureArea.h = 24;
//	animationStates[0].numberOfFrames = 1;
//	animationStates[0].frameOffset = 0;
//	animationStates[0].secondsPerFrame = new int[animationStates[0].numberOfFrames];
//	for (int j = 0; j < animationStates[0].numberOfFrames; ++j) {
//		animationStates[0].secondsPerFrame[j] = 1;
//	}
//
//	return true;
//}

bool Player::loadAnimationStates(std::ifstream* input) {
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

void Player::unloadAnimationStates() {
	if (animationStates) {
		delete[] animationStates;
		animationStates = NULL;
	}
}