#ifndef _CUSTOMIZESCREEN_H_
#define _CUSTOMIZESCREEN_H_

#include "MenuScreen.h"
#include "button.h"
#include <string>

struct HandicapBar {
	float textX, textY;
	hgeRect *collisionBox;
	hgeRect *bar;
	char tooltipText[20];
	bool mouseOver, clicked;
};

class CustomizeScreen : public MenuScreen {

public:

	CustomizeScreen();
	~CustomizeScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);
	void saveHandicaps();

	//Variables
	float guiX, guiY;
	float mouseX, mouseY;
	int selectedSong;

	//Objects
	Button *buttons[2];
	HandicapBar handicapBars[6];
	bool itemBarClicked[10];
	hgeRect *boardSizeBoxes[4], *nextSongBox, *previousSongBox, *playSongBox, *itemBars[10];

};

#endif