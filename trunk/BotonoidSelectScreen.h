#ifndef _BOTONOIDSELECTSCREEN_H_
#define _BOTONOIDSELECTSCREEN_H_

#include "game.h"
#include "MenuScreen.h"

struct SelectButton {
	float x, float y;
	bool highlighted;
	hgeRect *collisionBox;
	char text[50];
};

class BotonoidSelectScreen : public MenuScreen {

public:

	BotonoidSelectScreen();
	~BotonoidSelectScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	SelectButton buttons[2];

};

#endif