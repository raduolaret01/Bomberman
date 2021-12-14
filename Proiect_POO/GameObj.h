#pragma once

#include <fstream>
#include "TextureManager.h"
#include "SoundManager.h"

struct AnimationState {
	SDL_Rect textureArea;
	int numberOfFrames, frameOffset;
	int* secondsPerFrame;	

	AnimationState();
	~AnimationState();

	//bool operator==(const AnimationState&);
	//AnimationState& operator=(const AnimationState&);
};

class GameObj{
private:
	static int numberOfAnimStates;
	static AnimationState* animationStates;

protected:
	AnimationState* currentAnimState;
	SDL_Rect hitbox;
	int currentAnimFrame, currentFrameTime;

public:
	GameObj();
	GameObj(const SDL_Point,const int, const int);
	~GameObj();

	SDL_Rect getHitbox(void);

	virtual int getAnimState(void) = 0;
	virtual void setAnimState(int) = 0;
	virtual bool isOnLastAnimFrame(void) = 0;
	virtual int Show(SDL_Point) = 0;

	static bool loadAnimationStates(std::ifstream* input);
	static void unloadAnimationStates(void);

};
