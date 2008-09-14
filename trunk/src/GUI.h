#ifndef _GUI_H_
#define _GUI_H_

#include "game.h"

class GUI {

public:

	GUI();
	~GUI();

	//methods
	void draw(float dt);
	void drawTimer(float dt);
	void update(float dt);
	void startFaceAnimation(int whichPlayer);

	hgeSprite *barSprites[3];
	hgeSprite *windowSprites[3];
	hgeSprite *healthSprites[3];
	hgeAnimation *faceAnimations[3];
	hgeSprite *emptyFaceBoxes[3];

};

#endif