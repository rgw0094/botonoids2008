#ifndef _MENUSCREEN_H_
#define _MENUSCREEN_H_

#include "game.h"

class MenuScreen {

public:

	//Draw methods
	virtual void draw(float dt) { };
	virtual void update(float dt) { };
	virtual void clickMouse(float x, float y) { };

};

#endif