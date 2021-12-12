#pragma once

#include "GameObj.h"

class SolidObj : public GameObj {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;

protected:
	bool collisionFlag;

public:
	SolidObj();
	SolidObj(const SDL_Point,const int, const int);
	~SolidObj();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);

	bool getCollisionFlag(void);
	void setCollisionFlag(bool);
};

