#include "Game.h"
#include "BotonoidSelectScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern int mode;

#define BACK_BUTTON 0
#define NEXT_BUTTON 1

/**
 * Constructor.
 */
BotonoidSelectScreen::BotonoidSelectScreen() {

	//Create Buttons
	buttons[BACK_BUTTON] = new Button(100.0f, 650.0f, "Main Menu");
	buttons[NEXT_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "Continue");

}

/**
 * Destructor.
 */
BotonoidSelectScreen::~BotonoidSelectScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i];
	}
}

/**
 * Draw the botonoid select screen.
 */
void BotonoidSelectScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);

	//Draw header graphic
	resources->GetFont("timer")->printf(512,50,HGETEXT_CENTER, "Select Your Botonoid");

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

}

/**
 * Update the botonoid select screen.
 */
bool BotonoidSelectScreen::update(float dt, float mouseX, float mouseY) {

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		menu->currentScreen = TITLE_SCREEN;
	}
	
	//Click Next Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		menu->currentScreen = CUSTOMIZE_SCREEN;
	}

	return false;
}