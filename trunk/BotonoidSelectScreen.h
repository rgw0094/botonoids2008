#ifndef _BOTONOIDSELECTSCREEN_H_
#define _BOTONOIDSELECTSCREEN_H_

#include "MenuScreen.h"
#include "button.h"

class BotonoidSelectScreen : public MenuScreen {

public:

	BotonoidSelectScreen();
	~BotonoidSelectScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	Button *buttons[2];

};

#endif