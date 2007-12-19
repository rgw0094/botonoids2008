#include "game.h"

extern hgeResourceManager *resources;
extern HGE *hge;
extern int mode;

/**
 * Constructor
 */
Menu::Menu() {
	
	mouseX = mouseY = 400.0f;
	currentScreen = TITLE_SCREEN;

	//Create menu screen objects
	screens[TITLE_SCREEN] = new TitleScreen();
	screens[SELECT_SCREEN] = new BotonoidSelectScreen();

}

/**
 * Destructor
 */
Menu::~Menu() {
	for (int i = 0; i < NUM_SCREENS; i++) {
		delete screens[i];
	}
}

/** 
 * Main draw method. Calls the draw method for whichever screen is currently active.
 */
void Menu::draw(float dt) {
	
	//Draw current screen
	screens[currentScreen]->draw(dt);

	//Draw the mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

/**
 * Main update method. Calls the update method for whichever screen is currently active.
 */
bool Menu::update(float dt) {

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Update current screen
	return screens[currentScreen]->update(dt, mouseX, mouseY);

}
