#ifndef _CUSTOMIZESCREEN_H_
#define _CUSTOMIZESCREEN_H_

#include "MenuScreen.h"
#include "button.h"

struct ToolTip {
	float textX, textY;
	hgeRect *collisionBox;
	char text[20];
	bool mouseOver;
};

class CustomizeScreen : public MenuScreen {

public:

	CustomizeScreen();
	~CustomizeScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	//Variables
	float guiX, guiY;

	//Objects
	Button *buttons[2];
	ToolTip tooltips[6];

};

#endif