#include "Game.h"
#include "OptionsScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern int mode;

#define CANCEL_BUTTON 0
#define SAVE_BUTTON 1

/**
 * Constructor.
 */
OptionsScreen::OptionsScreen() {

	//Create Buttons
	buttons[CANCEL_BUTTON] = new Button(100.0f, 650.0f, "Cancel");
	buttons[SAVE_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "Save");

}

/**
 * Destructor.
 */
OptionsScreen::~OptionsScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i];
	}
}

/**
 * Draw the botonoid select screen.
 */
void OptionsScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);

	//Draw header graphic
	resources->GetFont("timer")->printf(512,50,HGETEXT_CENTER, "Options");

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

}

/**
 * Update the botonoid select screen.
 */
bool OptionsScreen::update(float dt, float mouseX, float mouseY) {

	//Update buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Click Cancel Button
	if (buttons[CANCEL_BUTTON]->isClicked()) {
		menu->currentScreen = TITLE_SCREEN;
	}

	//Click Save Button
	if (buttons[SAVE_BUTTON]->isClicked()) {
		//save
	}

	return false;
}