#ifndef _MENUSCREEN_H_
#define _MENUSCREEN_H_

#include "game.h"

class MenuScreen {

public:

	virtual void draw(float dt) { };
	virtual bool update(float dt, float mouseX, float mouseY) { return false; };

};

#endif