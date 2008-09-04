#ifndef _MINIMENU_H_
#define _MINIMENU_H_

#include "Button.h"
#include <list>

#define NUM_BUTTONS 2
#define QUIT_BUTTON 0
#define CONTINUE_BUTTON 1

class MiniMenu {

public:

	MiniMenu();
	~MiniMenu();

	//Draw methods
	void draw(float dt);
	void update(float dt);

	//Variables
	Button *buttons[NUM_BUTTONS];
	float mouseX, mouseY;
	bool active;

};

#endif