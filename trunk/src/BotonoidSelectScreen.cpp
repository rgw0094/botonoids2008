#include "Game.h"
#include "BotonoidSelectScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern Input *input;
extern GameInfo gameInfo;
extern std::string botonoidNames[3];
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
	
	//Load selecter graphics
	selecterSprites[0] = resources->GetSprite("selecter1");
	selecterSprites[1] = resources->GetSprite("selecter2");
	selecterSprites[2] = resources->GetSprite("selecter3");
	lockedSelecterSprites[0] = resources->GetSprite("lockedSelecter1");
	lockedSelecterSprites[1] = resources->GetSprite("lockedSelecter2");
	lockedSelecterSprites[2] = resources->GetSprite("lockedSelecter3");

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

	resetScreen();

}

/**
 * Destructor.
 */
BotonoidSelectScreen::~BotonoidSelectScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i];
	}
	for (int i = 0; i < 3; i++) {
		delete botonoids[i];
		delete highlightedBotonoids[i];
		delete selecterSprites[i];
		delete lockedSelecterSprites[i];
	}
}

/**
 * Draw the botonoid select screen.
 */
void BotonoidSelectScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);
	
	//Draw header graphic
	resources->GetFont("button")->SetScale(1.3);
	resources->GetFont("button")->printf(512,50,HGETEXT_CENTER, "select your botonoid");
	resources->GetFont("button")->SetScale(1.0);

	//Draw botonoids
	for (int i = 0; i < 3; i++) {
		if (botonoidSelected(i)) {
			highlightedBotonoids[i]->Render(botonoidPoints[i].x, botonoidPoints[i].y);
		} else {
			botonoids[i]->Render(botonoidPoints[i].x, botonoidPoints[i].y);
		}
	}

	//Botonoid names
	resources->GetFont("button")->printf(170.0, 240.0, HGETEXT_CENTER, botonoidNames[0].c_str());
	resources->GetFont("button")->printf(515.0, 525.0, HGETEXT_CENTER, botonoidNames[1].c_str());
	resources->GetFont("button")->printf(835.0, 185.0, HGETEXT_CENTER, "herbanoid");


	//Draw selecters
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		float x = points[i][selecters[i].selection].x;
		float y = points[i][selecters[i].selection].y;
		if (selecters[i].selected) {
			lockedSelecterSprites[i]->Render(x,y);
		} else {
			selecterSprites[i]->Render(x,y);
		}
		resources->GetFont("button")->printf(x+37.5f, y + 20.0f, HGETEXT_CENTER, "%dP", i+1);
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
	//if (allSelected()) buttons[NEXT_BUTTON]->update(mouseX, mouseY);
	buttons[NEXT_BUTTON]->update(mouseX, mouseY);	//temp for debug

	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		resetScreen();
		menu->currentScreen = TITLE_SCREEN;
	}
	
	//Click Next Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		if (allSelected()) {

			//Assign selected botonoids
			for (int i = 0; i < gameInfo.numPlayers; i++) {
				gameInfo.selectedBotonoid[i] = selecters[i].selection;
			}

			resetScreen();
			menu->currentScreen = CUSTOMIZE_SCREEN;

		} else {
			//temp for debugging
			for (int i = 0; i < gameInfo.numPlayers; i++) {
				gameInfo.selectedBotonoid[i] = i;
			}
			resetScreen();
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
				//Deselect
				selecters[player].selected = false;
			} else if (!botonoidSelected(selecters[player].selection)) {
				//Select
				selecters[player].selected = true;

				//Play selection sound
				switch (selecters[player].selection) {
					case ALPHANOID:
						hge->Effect_Play(resources->GetEffect("snd_alphanoidSelect"));
						break;
					case BARVINOID:
						hge->Effect_Play(resources->GetEffect("snd_barvinoidSelect"));
						break;
					case HERBANOID:
						hge->Effect_Play(resources->GetEffect("snd_herbanoidSelect"));
						break;
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
 * Resets the screen to it's initial status.
 */
void BotonoidSelectScreen::resetScreen() {
	for (int i = 0; i < 3; i++) {
		selecters[i].player = i;
		selecters[i].selected = false;
		selecters[i].selection = 0;
	}
	for (int i = 0; i < 2; i++) {
		buttons[i]->highlighted = false;
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


