#pragma once

#include <SDL.h>

class Timer {
protected:
	static Uint32 lastTime, dTime;

public:
	Timer();

	static int getDTime(void);
	static void setDTime(void);
};

