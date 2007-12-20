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

	//Create back button
	buttons[CANCEL_BUTTON].x = 100.0f;
	buttons[CANCEL_BUTTON].y = 650.0f;
	buttons[CANCEL_BUTTON].collisionBox = new hgeRect(buttons[CANCEL_BUTTON].x, buttons[CANCEL_BUTTON].y, buttons[CANCEL_BUTTON].x + BUTTON_WIDTH, buttons[CANCEL_BUTTON].y + BUTTON_HEIGHT);
	buttons[CANCEL_BUTTON].highlighted = false;
	strcpy(buttons[CANCEL_BUTTON].text, "Cancel");

	//Create next button
	buttons[SAVE_BUTTON].x = 1024.0f - 100.0f - BUTTON_WIDTH;
	buttons[SAVE_BUTTON].y = 650.0f;
	buttons[SAVE_BUTTON].collisionBox = new hgeRect(buttons[SAVE_BUTTON].x, buttons[SAVE_BUTTON].y, buttons[SAVE_BUTTON].x + BUTTON_WIDTH, buttons[SAVE_BUTTON].y + BUTTON_HEIGHT);
	buttons[SAVE_BUTTON].highlighted = false;
	strcpy(buttons[SAVE_BUTTON].text, "Save");

}

/**
 * Destructor.
 */
OptionsScreen::~OptionsScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i].collisionBox;
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

		//Button graphic
		if (buttons[i].highlighted) {
			resources->GetSprite("miniMenuButtonHighlighted")->Render(buttons[i].x, buttons[i].y);
		} else {
			resources->GetSprite("miniMenuButton")->Render(buttons[i].x, buttons[i].y);
		}

		//Button text
		resources->GetFont("timer")->printf(buttons[i].x + BUTTON_WIDTH/2, buttons[i].y + 20.0f, HGETEXT_CENTER, buttons[i].text);

	}

}

/**
 * Update the botonoid select screen.
 */
bool OptionsScreen::update(float dt, float mouseX, float mouseY) {

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i].highlighted = buttons[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Mouse click
	if (hge->Input_KeyDown(HGEK_LBUTTON)) {

		//Cancel Button
		if (buttons[CANCEL_BUTTON].highlighted) {
			menu->currentScreen = TITLE_SCREEN;
		}

		//Save Button
		if (buttons[SAVE_BUTTON].highlighted) {
			//save
		}

	}


	return false;
}