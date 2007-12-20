#ifndef _CUSTOMIZESCREEN_H_
#define _CUSTOMIZESCREEN_H_

#include "MenuScreen.h"
#include "button.h"

class CustomizeScreen : public MenuScreen {

public:

	CustomizeScreen();
	~CustomizeScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	Button *buttons[2];

};

#endif