#include "game.h"

extern hgeResourceManager *resources;
extern HGE *hge;
extern int mode;

Menu::Menu() {
	mouseX = mouseY = 400.0f;
	whichScreen = TITLE_SCREEN;
}

Menu::~Menu() {

}

void Menu::draw(float dt) {
	
	if (whichScreen == TITLE_SCREEN) {
		drawTitleScreen(dt);
	}

	//Draw the mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

void Menu::update(float dt) {

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	if (whichScreen == TITLE_SCREEN) {
		updateTitleScreen(dt);
	}

}

void Menu::drawTitleScreen(float dt) {
	resources->GetSprite("titlescreen")->Render(0,0);
}

void Menu::updateTitleScreen(float dt) {

	if (hge->Input_GetKeyState(HGEK_LBUTTON)) {
		mode = GAME_MODE;
	}	

}