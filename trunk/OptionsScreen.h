#ifndef _OPTIONSSCREEN_H_
#define _OPTIONSSCREEN_H_

#include "game.h"
#include "MenuScreen.h"

struct OptionsButton {
	float x, float y;
	bool highlighted;
	hgeRect *collisionBox;
	char text[50];
};

class OptionsScreen : public MenuScreen {

public:

	OptionsScreen();
	~OptionsScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	OptionsButton buttons[2];

};

#endif