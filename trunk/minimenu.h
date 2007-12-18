#ifndef _MINIMENU_H_
#define _MINIMENU_H_

#include "game.h"

//Menu pages
#define TITLE_SCREEN 0

class MiniMenu {

public:

	MiniMenu();
	~MiniMenu();

	//Draw methods
	void draw(float dt);
	void update(float dt);

	//Variables
	float mouseX, mouseY;
	bool active;

};

#endif