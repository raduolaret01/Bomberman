#pragma once
#include "SolidObj.h"
class Obstacle : public SolidObj {
protected:
	static int numberOfAnimStates;

public:
	enum AnimStatesEnum {
		Default,
		BlowingUp
	};

	Obstacle();
	Obstacle(const SDL_Point, const int, const int);
	~Obstacle();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);

};

class Obstacle1 : public Obstacle {
private:
	static AnimationState* animationStates;

public:
	enum AnimStatesEnum {
		Default,
		BlowingUp
	};

	Obstacle1();
	Obstacle1(const SDL_Point, const int, const int);
	~Obstacle1();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);


	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

class Obstacle2 : public Obstacle {
private:
	static AnimationState* animationStates;

public:
	enum AnimStatesEnum {
		Default,
		BlowingUp
	};

	Obstacle2();
	Obstacle2(const SDL_Point, const int, const int);
	~Obstacle2();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);


	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};
