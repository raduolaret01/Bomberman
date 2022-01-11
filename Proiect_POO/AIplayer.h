#pragma once
#include "Player.h"
class AIplayer : public Player {
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;

public:
	enum AIStateEnum {
		Default,
		InRangeOfBomb,
		NoEscape
	};
	int state;
	const int reactionDelay = 150;
	int reactionTime;

	AIplayer();
	AIplayer(const SDL_Point, const int, const int);
	~AIplayer();

	int getAnimState(void);
	void setAnimState(int);
	bool isOnLastAnimFrame(void);
	int Show(SDL_Point);

	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);
};

