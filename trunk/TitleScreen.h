#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include "game.h"
#include "MenuScreen.h"

class TitleScreen : public MenuScreen {

public:

	TitleScreen();
	~TitleScreen();

	//Draw methods
	void draw(float dt);
	void update(float dt);
	void clickMouse(float x, float y);

};

#endif