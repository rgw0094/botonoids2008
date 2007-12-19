#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

#include "game.h"


struct TitleMenuButton {
	float x, y;
	hgeRect *collisionBox;
	bool highlighted;
	char text[50];
};

class TitleMenu {

public:

	TitleMenu();
	~TitleMenu();

	//Draw methods
	void draw(float dt);
	void update(float dt);
	void clickMouse(float x, float y);

	//Variables
	TitleButton buttons[4];

};

#endif