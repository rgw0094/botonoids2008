#ifndef _OPTIONSSCREEN_H_
#define _OPTIONSSCREEN_H_

#include "MenuScreen.h"
#include "button.h"

class OptionsScreen : public MenuScreen {

public:

	OptionsScreen();
	~OptionsScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	//Variables
	float guiX, guiY;
	float mouseX, mouseY;
	bool musicBarPressed;
	bool soundBarPressed;

	//Objects
	Button *buttons[2];
	hgeRect *inputBox, *soundVolumeBox, *musicVolumeBox;

};

#endif