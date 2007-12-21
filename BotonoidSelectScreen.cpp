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

	//Load botonoid graphics
	botonoids[0] = resources->GetSprite("selectGold");
	botonoids[1] = resources->GetSprite("selectBlack");
	botonoids[2] = resources->GetSprite("selectWhite");
	highlightedBotonoids[0] = resources->GetSprite("selectGoldHighlighted");
	highlightedBotonoids[1] = resources->GetSprite("selectBlackHighlighted");
	highlightedBotonoids[2] = resources->GetSprite("selectWhiteHighlighted");
	
	//Place botonoids
	botonoidPoints[0].x = 30.0f;
	botonoidPoints[0].y = 270.0f;
	botonoidPoints[1].x = 30.0f + 338.0f;
	botonoidPoints[1].y = 130.0f;
	botonoidPoints[2].x = 30.0f + 690.0f;
	botonoidPoints[2].y = 210.0f;

	//Selection points (i==player, j==selection)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			float y;
			if (j == 0) y = 540.0f;
			if (j == 1) y = 440.0f;
			if (j == 2) y = 540.0f;
			points[i][j].x = 50.0f + 350.0f*j + 90.0f*i;
			points[i][j].y = y;
		}
	}

	resetSelecters();

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
	resources->GetFont("button")->printf(512,50,HGETEXT_CENTER, "Select Your Botonoid");

	//Draw botonoids
	for (int i = 0; i < 3; i++) {
		if (botonoidSelected(i)) {
			highlightedBotonoids[i]->Render(botonoidPoints[i].x, botonoidPoints[i].y);
		} else {
			botonoids[i]->Render(botonoidPoints[i].x, botonoidPoints[i].y);
		}
	}

	//Draw selecters
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		float x = points[i][selecters[i].selection].x;
		float y = points[i][selecters[i].selection].y;
		if (selecters[i].selected) {
			resources->GetSprite("lockedSelecter")->Render(x,y);
		} else {
			resources->GetSprite("selecter")->Render(x,y);
		}
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
	buttons[BACK_BUTTON]->update(mouseX, mouseY);
	if (allSelected()) buttons[NEXT_BUTTON]->update(mouseX, mouseY);

	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		resetSelecters();
		menu->currentScreen = TITLE_SCREEN;
	}
	
	//Click Next Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		if (allSelected()) {

			//Assign selected botonoids
			for (int i = 0; i < gameInfo.numPlayers; i++) {
				gameInfo.selectedBotonoid[i] = selecters[i].selection;
			}

			resetSelecters();
			menu->currentScreen = CUSTOMIZE_SCREEN;

		}
	}


	//Input
	for (int player = 0; player < gameInfo.numPlayers; player++) {

		//Move selecters if not locked
		if (!selecters[player].selected) {

			//Left
			if (input->buttonPressed(INPUT_LEFT, player)) {
				if (selecters[player].selection == 0) 
					selecters[player].selection = 2;
				else
					selecters[player].selection--;

			//Right
			} else if (input->buttonPressed(INPUT_RIGHT, player)) {
				if (selecters[player].selection == 2) 
					selecters[player].selection = 0;
				else
					selecters[player].selection++;
			}

		}

		//Select current botonoid
		if (input->buttonPressed(INPUT_ACTION, player)) {
			if (selecters[player].selected) {
				selecters[player].selected = false;
			} else {
				if (!botonoidSelected(selecters[player].selection)) {
					selecters[player].selected = true;
				}
			}
		}

	}

	return false;
}

/**
 * Returns whether or not all players have selected a Botonoid.
 */
bool BotonoidSelectScreen::allSelected() {
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (!selecters[i].selected) return false;
	}
	return true;
}

/**
 * Resets the selecters.
 */
void BotonoidSelectScreen::resetSelecters() {
	for (int i = 0; i < 3; i++) {
		selecters[i].player = i;
		selecters[i].selected = false;
		selecters[i].selection = 0;
	}
}

/**
 * Returns whether or not the specified botonoid has already been selected.
 */
bool BotonoidSelectScreen::botonoidSelected(int botonoid) {
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (selecters[i].selection == botonoid && selecters[i].selected) return true;
	}
	return false;
}	


