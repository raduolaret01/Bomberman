#pragma once

#include "TextureManager.h"
#include "Button.h"

class Menu {
protected:
	char* title;
	static bool quitFlag;
public:
	Button** ButtonArray;
	bool isShown;

	Menu();
	~Menu();
	bool getQuitFlag(void);
	static void setQuitFlag(void);
	virtual void Init(void) = 0;
	virtual void Free(void) = 0;
	virtual void Show(void) = 0;
	void Hide(void);
};

class MainMenu :public Menu {
public:
	enum Buttons {
		Quit,
		Load_1p_1,
		Load_1p_2,
		Load_1p_3,
		Load_2p_1,
		Load_2p_2
	};

	MainMenu();
	~MainMenu();
	void Init(void);
	void Free(void);
	void Show(void);
};
//
//class PauseMenu :public Menu {
//public:
//	enum Buttons {
//		Quit,
//		QuitToMenu,
//		Resume
//	};
//
//	void Init(void);
//	void Free(void);
//	void Show(void);
//	void Hide(void);
//};

