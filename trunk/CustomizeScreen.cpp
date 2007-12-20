#include "Game.h"
#include "CustomizeScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern int mode;

#define BACK_BUTTON 0
#define NEXT_BUTTON 1

/**
 * Constructor.
 */
CustomizeScreen::CustomizeScreen() {

	//Create buttons
	buttons[BACK_BUTTON] = new Button(100.0f, 650.0f, "Back");
	buttons[NEXT_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "Start Game");

}

/**
 * Destructor.
 */
CustomizeScreen::~CustomizeScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i];
	}
}

/**
 * Draw the botonoid select screen.
 */
void CustomizeScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);

	//Draw header graphic
	resources->GetFont("timer")->printf(512,50,HGETEXT_CENTER, "Customize Game");

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

}

/**
 * Update the botonoid select screen.
 */
bool CustomizeScreen::update(float dt, float mouseX, float mouseY) {

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		menu->currentScreen = SELECT_SCREEN;
	}

	//Click Next Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		startGame();
	}

	return false;
}