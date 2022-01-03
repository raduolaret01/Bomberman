#pragma once

#include "SolidObj.h"
#include "Player.h"

class Bomb : public SolidObj {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;
protected:
	int range;
	Player* owner;
public:
	enum AnimStatesEnum {
		Tick,
		Explosion
	};

	Bomb();
	Bomb(const SDL_Point, const int, const int, Player*);
	~Bomb();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);


	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

