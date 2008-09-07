#ifndef _BOTONOIDSELECTSCREEN_H_
#define _BOTONOIDSELECTSCREEN_H_

#include "MenuScreen.h"
#include "button.h"
#include "game.h"

struct Point {
	float x,y;
};

struct Selecter {
	int player;
	int selection;
	bool selected;
};

class BotonoidSelectScreen : public MenuScreen {

public:

	BotonoidSelectScreen();
	~BotonoidSelectScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);
	void resetScreen();
	bool allSelected();
	bool botonoidSelected(int botonoid);

	Button *buttons[2];
	hgeSprite *botonoids[3];
	hgeSprite *selecterSprites[3];
	hgeSprite *lockedSelecterSprites[3];

	Selecter selecters[3];
	Point botonoidPoints[3];
	Point points[3][3];

};

#endif