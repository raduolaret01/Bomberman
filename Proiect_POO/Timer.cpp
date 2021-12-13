#include "Timer.h"

Uint32 Timer::lastTime = 0;
Uint32 Timer::dTime = 0;

Timer::Timer() {
	lastTime = SDL_GetTicks();
}

int Timer::getDTime() {
	return dTime;
}

void Timer::setDTime() {
	Uint32 thisTime = SDL_GetTicks();
	dTime = thisTime - lastTime;
	lastTime = thisTime;
}