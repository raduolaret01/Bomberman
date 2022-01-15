#pragma once

#include "TextureManager.h"
#include "SoundManager.h"
#include "Button.h"

class Menu {
protected:
	//Flags are necessary to avoid circular dependecy with Game
	static bool quitFlag;
	static int loadLevelFlag;
	static bool reloadFlag;
	static bool isInMainMenuFlag;
	static bool isInPauseMenuFlag;
	static bool replayLevelFlag;

public:

	SDL_Texture* Background;
	Button** ButtonArray;

	Menu();
	~Menu();

#pragma region FlagFunctions
	static void clearFlags(void);
	bool getQuitFlag(void);
	static void setQuitFlag(void);
	int getLoadLevelFlag(void);
	static void loadLevel1(void);
	static void loadLevel2(void);
	static void loadLevel3(void);
	static void loadLevel4(void);
	static void loadLevel5(void);
	bool getReloadFlag(void);
	static void setReloadFlag(void);
	bool getMainMenuFlag(void);
	static void setMainMenuFlag(void);
	bool getPauseMenuFlag(void);
	static void setPauseMenuFlag(void);
	static void clearPauseMenuFlag(void);
	bool getReplayLevelFlag(void);
	static void setReplayLevelFlag(void);
	static void clearReplayLevelFlag(void);
#pragma endregion

	virtual void Init(void) = 0;
	virtual void Show(void) = 0;
	virtual void Hide(void) = 0;
	virtual void Destroy(void) = 0;
};

class MainMenu :public Menu {
public:
	enum Buttons {
		Quit,
		Load_1p_1,
		Load_1p_2,
		Load_1p_3,
		Load_2p_coop,
		Load_2p_vs
	};

	MainMenu();
	~MainMenu();
	void Init(void);
	void Show(void);
	void Hide(void);
	void Destroy(void);
};

class PauseMenu :public Menu {
public:
	enum Buttons {
		Quit,
		QuitToMenu,
		Resume
	};

	PauseMenu();
	~PauseMenu();
	void Init(void);
	void Show(void);
	void Hide(void);
	void Destroy(void);
};

class GameOverMenu : public Menu {
public:
	enum Buttons {
		Quit,
		QuitToMenu,
		Retry
	};

	GameOverMenu();
	~GameOverMenu();
	void Init(void);
	void Show(void);
	void Hide(void);
	void Destroy(void);

};

class VictoryMenu : public Menu {
public:
	enum Buttons {
		Quit,
		QuitToMenu,
		Replay
	};

	VictoryMenu();
	~VictoryMenu();
	void Init(void);
	void Show(void);
	void Hide(void);
	void Destroy(void);
};