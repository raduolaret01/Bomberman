#include "Level.h"

Level::Level() {
	map = NULL;
	objectCount = NULL;
}

Level::~Level() {
	if (map) {
		for (int i = 0; i < mapH; ++i) {
			if (map[i]) {
				delete[] map[i];
				map[i] = NULL;
			}
		}
		delete[] map;
		map = NULL;
	}
	if (objectCount) {
		delete[] objectCount;
		objectCount = NULL;
	}
}