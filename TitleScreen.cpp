#include "game.h"
#include "TitleScreen.h"
#include "MenuScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern int mode;
extern GameInfo gameInfo;
extern Menu *menu;

/**
 * Constructor
 */
TitleScreen::TitleScreen() {

	//Left Petal - Options
	buttons[TITLE_OPTIONS].collisionBox = new hgeRect(270,632, 450,729);
	buttons[TITLE_OPTIONS].highlighted = false;
	strcpy(buttons[TITLE_OPTIONS].text, "Options");
	
	//Left Middle Petal - 2 Players
	buttons[TITLE_2P].collisionBox = new hgeRect(371,478, 508,627);
	buttons[TITLE_2P].highlighted = false;
	strcpy(buttons[TITLE_2P].text, "2P");

	//Right Middle Petal - 3 Players
	buttons[TITLE_3P].collisionBox = new hgeRect(525,478, 661,627);
	buttons[TITLE_3P].highlighted = false;
	strcpy(buttons[TITLE_3P].text, "3P");

	//Right Petal - Quit
	buttons[TITLE_QUIT].collisionBox = new hgeRect(581,632, 769,729);
	buttons[TITLE_QUIT].highlighted = false;
	strcpy(buttons[TITLE_QUIT].text, "Quit");

}

/** 
 * Destructor
 */
TitleScreen::~TitleScreen() {
	for (int i = 0; i < 4; i++) {
		delete buttons[i].collisionBox;
	}
}

/**
 * Draws the title screen.
 */
void TitleScreen::draw(float dt) {
	
	//Draw the title screen graphic
	resources->GetSprite("titlescreen")->Render(0,0);

	//Draw header graphic
	resources->GetFont("timer")->printf(512,50,HGETEXT_CENTER, "Botonoids 2008");

	//Draw the buttons on the petals
	for (int i = 0; i < 4; i++) {
		
		//Button Background???
		if (buttons[i].highlighted) {
			drawCollisionBox(buttons[i].collisionBox, 255,0,0);
		} else {
			drawCollisionBox(buttons[i].collisionBox, 0,255,0);
		}
	
		//Button text
		float textX = buttons[i].collisionBox->x1 + (buttons[i].collisionBox->x2 - buttons[i].collisionBox->x1)/2;
		float textY = buttons[i].collisionBox->y1 + (buttons[i].collisionBox->y2 - buttons[i].collisionBox->y1)/2 - 10.0f;
		resources->GetFont("timer")->printf(textX, textY, HGETEXT_CENTER, buttons[i].text);

	}

}

/**
 * Updates the title screen
 */
bool TitleScreen::update(float dt, float mouseX, float mouseY) {

	//Check for highlighted buttons
	for (int i = 0; i < 4; i++) {
		buttons[i].highlighted = buttons[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Listen for mouse click
	if (hge->Input_KeyDown(HGEK_LBUTTON)) {

		//Options button
		if (buttons[TITLE_OPTIONS].highlighted) {
			menu->currentScreen = OPTIONS_SCREEN;
		}

		//2 Player button
		if (buttons[TITLE_2P].highlighted) {
			menu->currentScreen = SELECT_SCREEN;
			gameInfo.numPlayers = 2;
		}

		//3 Player button
		if (buttons[TITLE_3P].highlighted) {
			menu->currentScreen = SELECT_SCREEN;
			gameInfo.numPlayers = 3;
		}

		//Quit
		if (buttons[TITLE_QUIT].highlighted) {
			return true;
		}

	}	

	return false;

}