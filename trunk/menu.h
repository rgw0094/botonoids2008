#ifndef _MENU_H_
#define _MENU_H_

#include "game.h"
#include "titlescreen.h"
#include "BotonoidSelectScreen.h"
#include "CustomizeScreen.h"
#include "OptionsScreen.h"

//Menu Screens
#define NUM_SCREENS 4
#define TITLE_SCREEN 0
#define SELECT_SCREEN 1
#define CUSTOMIZE_SCREEN 2
#define OPTIONS_SCREEN 3


class Menu {

public:

	Menu();
	~Menu();

	//Draw methods
	void draw(float dt);
	bool update(float dt);
	void returnToMenu();

	//Variables
	MenuScreen *screens[NUM_SCREENS];
	float mouseX, mouseY;
	int currentScreen;

};

#endif