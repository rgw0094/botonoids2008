#include "game.h"

extern hgeResourceManager *resources;
extern HGE *hge;
extern int mode;

Menu::Menu() {
	mouseX = mouseY = 400.0f;
	whichScreen = TITLE_SCREEN;

	//Setup main menu buttons

}

Menu::~Menu() {

}

/** 
 * Main draw method. Calls the draw method for whichever screen is currently active.
 */
void Menu::draw(float dt) {
	
	if (whichScreen == TITLE_SCREEN) {
		drawTitleScreen(dt);
	}

	//Draw the mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

/**
 * Main update method. Calls the update method for whichever screen is currently active.
 */
void Menu::update(float dt) {

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	if (whichScreen == TITLE_SCREEN) {
		updateTitleScreen(dt);
	}

}

/********************************************************
 * TITLE SCREEN											*
 ********************************************************/ 
void Menu::drawTitleScreen(float dt) {
	resources->GetSprite("titlescreen")->Render(0,0);
}

void Menu::updateTitleScreen(float dt) {
	if (hge->Input_KeyDown(HGEK_LBUTTON)) {
		mode = GAME_MODE;
	}	
}