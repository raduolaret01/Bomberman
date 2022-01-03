#include <cmath>
#include "Player.h"

int Player::numberOfAnimStates = 6;
AnimationState* Player::animationStates = NULL;

Player::Player() : SolidObj() {
	range = 1;
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
	placedBombs = 0;
	maxPlacedBombs = 1;
}

Player::Player(const SDL_Point coord, const int w, const int h) : SolidObj(coord, w, h) {
	range = 1;
	direction = 0;
	currentAnimState = animationStates;
	speed = 0;
	placedBombs = 0;
	maxPlacedBombs = 1;
}

Player::~Player() {
	unloadAnimationStates();
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
	SDL_Rect TempDest = { (int)((hitbox.x - 2) * 2 + Offset.x),(int)((hitbox.y - 12) * 2 + Offset.y),currentAnimState->textureArea.w * 2,currentAnimState->textureArea.h * 2 };
	SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::Player1], &TempSrc, &TempDest,0,NULL,flip);

	//SDL_RenderCopyEx(TextureManager::Renderer, TextureManager::Texture[TextureManager::Player1], &TempSrc, &hitbox, 0, NULL, flip);
	return 0;
}

void Player::Update(int dTime, float collisionDist, bool dirChanged, bool spdChanged) {
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
		if(collisionDist!=-1.0f){
			switch (direction) {
			case Down:
				hitbox.y += std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Right:
				hitbox.x += std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Left:
				hitbox.x -= std::min(speed * dTime / 40.0f, collisionDist);
				break;
			case Up:
				hitbox.y -= std::min(speed * dTime / 40.0f, collisionDist);
				break;
			}
		}
		else {
			switch (direction) {
			case Down:
				hitbox.y += speed * dTime / 40.0f;
				break;
			case Right:
				hitbox.x += speed * dTime / 40.0f;
				break;
			case Left:
				hitbox.x -= speed * dTime / 40.0f;
				break;
			case Up:
				hitbox.y -= speed * dTime / 40.0f;
				break;
			}
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
	//printf("%d\n", dTime);
	//int mapX = (int)getHitbox().x / 16, mapY = (int)getHitbox().y / 16;
	//printf("%d,%d\n", mapY, mapX);
	
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

int Player::getDirection() {
	return direction;
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