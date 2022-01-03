#pragma once
#include "SolidObj.h"
class Player : public SolidObj {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;
protected:
	int direction;
	int speed;
public:
	int range, placedBombs, maxPlacedBombs;

	enum DirectionEnum {
		Down,
		Right,
		Up,
		Left
	};
	enum AnimStatesEnum {
		DownStill,
		SideStill,
		UpStill,
		DownMove,
		SideMove,
		UpMove,
	};

	Player();
	Player(const SDL_Point, const int, const int);
	~Player();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);

	bool setSpeed(int);
	bool setDirection(int);
	int getDirection(void);

	void Update(int, float, bool, bool);

	//static bool loadAnimationStates(void);
	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

