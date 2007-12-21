#ifndef _BOTONOIDSELECTSCREEN_H_
#define _BOTONOIDSELECTSCREEN_H_

#include "MenuScreen.h"
#include "button.h"
#include "game.h"

struct Selecter {
	int player;
	int selection;
	bool selected;
};

struct Point {
	float x,y;
};

class BotonoidSelectScreen : public MenuScreen {

public:

	BotonoidSelectScreen();
	~BotonoidSelectScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);
	void resetSelecters();
	bool allSelected();
	bool botonoidSelected(int botonoid);

	Button *buttons[2];
	hgeSprite *botonoids[3];
	hgeSprite *highlightedBotonoids[3];

	Selecter selecters[3];
	Point points[3][3];
	Point botonoidPoints[3];

};

#endif