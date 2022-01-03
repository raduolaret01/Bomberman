#include "SolidObj.h"

int SolidObj::numberOfAnimStates = 0;
AnimationState* SolidObj::animationStates = NULL;

SolidObj::SolidObj() : GameObj() {
	collisionFlag = true;
}

SolidObj::SolidObj(const SDL_Point coord, const int w, const int h) : GameObj(coord, w, h) {
	collisionFlag = true;
}

SolidObj::~SolidObj() {
	unloadAnimationStates();
}

int SolidObj::getAnimState() {
	return 0;
}

void SolidObj::setAnimState(int x) {
	return;
}

bool SolidObj::isOnLastAnimFrame() {
	return false;
}

int SolidObj::Show(SDL_Point Offset) {
	return 0;
}

bool SolidObj::getCollisionFlag() {
	return collisionFlag;
}

void SolidObj::setCollisionFlag(bool flag) {
	collisionFlag = flag;
}