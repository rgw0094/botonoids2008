#ifndef _MENU_H_
#define _MENU_H_

#include "game.h"
#include "titlescreen.h"

//Menu pages
#define NUM_SCREENS 1
#define TITLE_SCREEN 0

class Menu {

public:

	Menu();
	~Menu();

	//Draw methods
	void draw(float dt);
	bool update(float dt);

	//Variables
	MenuScreen *screens[NUM_SCREENS];
	float mouseX, mouseY;
	int currentScreen;

};

#endif