#ifndef _MENU_H_
#define _MENU_H_

#include "game.h"

//Menu pages
#define TITLE_SCREEN 0

class Menu {

public:

	Menu();
	~Menu();

	//Draw methods
	void draw(float dt);
	void drawTitleScreen(float dt);

	//Update methods
	void update(float dt);
	void updateTitleScreen(float dt);

	//Variables
	float mouseX, mouseY;
	int whichScreen;

};

#endif