#include "SoundManager.h"

Mix_Music* SoundManager::BGM[2] = { NULL };
Mix_Chunk* SoundManager::SFX[10] = { NULL };

SoundManager::SoundManager() {
	return;
}

SoundManager::~SoundManager() {
	for (int i = 0; i < 2; ++i) {
		if (BGM[i])
			Mix_FreeMusic(BGM[i]);
		BGM[i] = NULL;
	}
	for (int i = 0; i < 10; ++i) {
		if (SFX[i])
			Mix_FreeChunk(SFX[i]);
		SFX[i] = NULL;
	}
}

Mix_Chunk* SoundManager::loadSoundEffect(const char* path) {
	Mix_Chunk* newSFX = Mix_LoadWAV(path);
	if (newSFX == NULL) {
		printf("Error loading sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	return newSFX;
}

bool SoundManager::loadSFX() {
	SFX[Splash] = loadSoundEffect("Resources/sus.mp3");
	if (SFX[(int)SFXEnum::Splash] == NULL) {
		return false;
	}
	return true;
}

Mix_Music* SoundManager::loadMusic(const char* path) {
	Mix_Music* newMusic = Mix_LoadMUS(path);
	if (newMusic == NULL) {
		printf("Error loading music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	return newMusic;
}

bool SoundManager::loadBGM() {
	return true;
}

void SoundManager::freeSounds() {
	for (int i = 0; i < 2; ++i) {
		if (BGM[i])
			Mix_FreeMusic(BGM[i]);
		BGM[i] = NULL;
	}
	for (int i = 0; i < 10; ++i) {
		if (SFX[i])
			Mix_FreeChunk(SFX[i]);
		SFX[i] = NULL;
	}
}
