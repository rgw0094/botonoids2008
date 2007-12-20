#include "Game.h"
#include "BotonoidSelectScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern Input *input;
extern GameInfo gameInfo;
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

	botonoids[0] = resources->GetSprite("selectGold");
	botonoids[1] = resources->GetSprite("selectBlack");
	botonoids[2] = resources->GetSprite("selectWhite");

	//Selection points (i==player, j==selection)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			float y;
			if (j == 0) y = 500.0f;
			if (j == 1) y = 400.0f;
			if (j == 2) y = 500.0f;
			points[i][j].x = 100.0f + 350.0f*j + 90.0f*i;
			points[i][j].y = y;
		}
	}

	//Selecters
	for (int i = 0; i < 3; i++) {
		selecters[i].player = i;
		selecters[i].selected = false;
		selecters[i].selection = 0;
	}

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
	//hge->Gfx_Clear(0);
	
	//Draw header graphic
	resources->GetFont("timer")->printf(512,50,HGETEXT_CENTER, "Select Your Botonoid");

	//Draw botonoids
	for (int i = 0; i < 3; i++) {
		float y;
		if (i == 0) y = 270.0f;
		if (i == 1) y = 140.0f;
		if (i == 2) y = 190.0f;
		botonoids[i]->Render(30.0f + 350.0f*i, y);
	}

	//Draw selecters
	for (int i = 0; i < 3; i++) {
		float x = points[i][selecters[i].selection].x;
		float y = points[i][selecters[i].selection].y;
		resources->GetSprite("selecter")->Render(x,y);
		resources->GetFont("button")->printf(x+32.5f, y + 10.0f, HGETEXT_CENTER, "%dP", i+1);
	}

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


	//Input
	for (int player = 0; player < gameInfo.numPlayers; player++) {

		//Move selecter left
		if (input->buttonPressed(INPUT_LEFT, player)) {
			if (selecters[player].selection == 0) 
				selecters[player].selection = 2;
			else
				selecters[player].selection--;

		//Move selecter right
		} else if (input->buttonPressed(INPUT_RIGHT, player)) {
			if (selecters[player].selection == 2) 
				selecters[player].selection = 0;
			else
				selecters[player].selection++;
		}

	}



	return false;
}

