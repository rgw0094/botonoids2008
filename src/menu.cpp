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
	screens[CUSTOMIZE_SCREEN] = new CustomizeScreen();
	screens[OPTIONS_SCREEN] = new OptionsScreen();

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
	if (hge->Input_IsMouseOver()) resources->GetSprite("mouse")->Render(mouseX, mouseY);

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

/**
 * Returns to the MENU
 */
void Menu::returnToMenu() {
	mode = MENU_MODE;
	currentScreen = TITLE_SCREEN;
	setMusic("menu");
}


