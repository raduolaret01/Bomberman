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
		Logs::logF << "Error loading sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return newSFX;
}

bool SoundManager::loadSFX() {
	SFX[Splash] = loadSoundEffect("Resources/Sounds/sus.mp3");
	if (SFX[Splash] == NULL) {
		return false;
	}
	SFX[MouseOver] = loadSoundEffect("Resources/Sounds/Button_MouseOver.wav");
	if (SFX[MouseOver] == NULL) {
		return false;
	}
	SFX[Click] = loadSoundEffect("Resources/Sounds/Button_Click.wav");
	if (SFX[Click] == NULL) {
		return false;
	}
	SFX[Bomb_Place] = loadSoundEffect("Resources/Sounds/Bomb_Place.wav");
	if (SFX[Bomb_Place] == NULL) {
		return false;
	}
	SFX[Bomb_Boom] = loadSoundEffect("Resources/Sounds/Bomb_Explosion.wav");
	if (SFX[Bomb_Boom] == NULL) {
		return false;
	}
	Mix_VolumeChunk(SFX[Bomb_Boom], 64);
	SFX[Step] = loadSoundEffect("Resources/Sounds/Footstep.wav");
	if (SFX[Step] == NULL) {
		return false;
	}
	SFX[NPC_Death] = loadSoundEffect("Resources/Sounds/NPC_Death.wav");
	if (SFX[NPC_Death] == NULL) {
		return false;
	}
	SFX[Player_Death] = loadSoundEffect("Resources/Sounds/Player_Death.wav");
	if (SFX[Player_Death] == NULL) {
		return false;
	}
	return true;
}

Mix_Music* SoundManager::loadMusic(const char* path) {
	Mix_Music* newMusic = Mix_LoadMUS(path);
	if (newMusic == NULL) {
		Logs::logF << "Error loading music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return newMusic;
}

bool SoundManager::loadBGM() {
	BGM[Menu] = loadMusic("Resources/Sounds/MainMenu.mp3");
	BGM[Game] = loadMusic("Resources/Sounds/Game.mp3");
	Mix_VolumeMusic(64);
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
