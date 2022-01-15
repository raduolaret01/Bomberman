#pragma once

#include "SolidObj.h"
#include "Player.h"

class Blast : public GameObj {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;

protected:
	int direction;
public:
	enum DirectionEnum {
		Down,
		Right,
		Up,
		Left
	};

	enum AnimStatesEnum {
		VerticalTip,
		VerticalMiddle,
		HorizontalTip,
		HorizontalMiddle
	};

	Blast();
	Blast(const SDL_Point, const int, const int, const int);
	~Blast();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);

	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

class Bomb : public SolidObj {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;
protected:
	int range,blastCount;
	Blast** blast;
public:
	Player* owner;
	int tick;
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

	int getRange();
	void setBlast(Blast**,int);

	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

