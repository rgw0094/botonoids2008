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

	//Create back button
	buttons[BACK_BUTTON].x = 100.0f;
	buttons[BACK_BUTTON].y = 650.0f;
	buttons[BACK_BUTTON].collisionBox = new hgeRect(buttons[BACK_BUTTON].x, buttons[BACK_BUTTON].y, buttons[BACK_BUTTON].x + BUTTON_WIDTH, buttons[BACK_BUTTON].y + BUTTON_HEIGHT);
	buttons[BACK_BUTTON].highlighted = false;
	strcpy(buttons[BACK_BUTTON].text, "Back");

	//Create next button
	buttons[NEXT_BUTTON].x = 1024.0f - 100.0f - BUTTON_WIDTH;
	buttons[NEXT_BUTTON].y = 650.0f;
	buttons[NEXT_BUTTON].collisionBox = new hgeRect(buttons[NEXT_BUTTON].x, buttons[NEXT_BUTTON].y, buttons[NEXT_BUTTON].x + BUTTON_WIDTH, buttons[NEXT_BUTTON].y + BUTTON_HEIGHT);
	buttons[NEXT_BUTTON].highlighted = false;
	strcpy(buttons[NEXT_BUTTON].text, "Next");

}

/**
 * Destructor.
 */
BotonoidSelectScreen::~BotonoidSelectScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i].collisionBox;
	}
}

/**
 * Draw the botonoid select screen.
 */
void BotonoidSelectScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);

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
bool BotonoidSelectScreen::update(float dt, float mouseX, float mouseY) {

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i].highlighted = buttons[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Mouse click
	if (hge->Input_KeyDown(HGEK_LBUTTON)) {

		//Back Button
		if (buttons[BACK_BUTTON].highlighted) {
			menu->currentScreen = TITLE_SCREEN;
		}

		//Next Button
		if (buttons[NEXT_BUTTON].highlighted) {
			startGame();
		}

	}


	return false;
}