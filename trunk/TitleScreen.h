#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include "game.h"
#include "MenuScreen.h"

//Title screen buttons
#define TITLE_OPTIONS 0
#define TITLE_2P 1
#define TITLE_3P 2
#define TITLE_QUIT 3

struct TitleButton {
	float x, y;
	hgeRect *collisionBox;
	bool highlighted;
};

class TitleScreen : public MenuScreen {

public:

	TitleScreen();
	~TitleScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	TitleButton buttons[4];

};

#endif