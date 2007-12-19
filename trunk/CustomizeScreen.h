#ifndef _CUSTOMIZESCREEN_H_
#define _CUSTOMIZESCREEN_H_

#include "game.h"
#include "MenuScreen.h"

struct CustomizeButton {
	float x, float y;
	bool highlighted;
	hgeRect *collisionBox;
	char text[50];
};

class CustomizeScreen : public MenuScreen {

public:

	CustomizeScreen();
	~CustomizeScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	CustomizeButton buttons[2];

};

#endif