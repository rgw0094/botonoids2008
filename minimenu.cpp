#include "game.h"

extern hgeResourceManager *resources;
extern HGE *hge;
extern int mode;
extern Menu *menu;


/**
 * Constructor.
 */
MiniMenu::MiniMenu() {

	mouseX = mouseY = 400.0f;
	active = false;

	//Create the buttons
	buttons[QUIT_BUTTON] = new Button((1024 - BUTTON_WIDTH) / 2, (768 - NUM_BUTTONS*BUTTON_HEIGHT + (NUM_BUTTONS-1)*25) / 2 + BUTTON_HEIGHT*0 + 25*0, "Quit");
	buttons[CONTINUE_BUTTON] = new Button((1024 - BUTTON_WIDTH) / 2, (768 - NUM_BUTTONS*BUTTON_HEIGHT + (NUM_BUTTONS-1)*25) / 2 + BUTTON_HEIGHT*1 + 25*1, "Continue");

}

/**
 * Destructor.
 */
MiniMenu::~MiniMenu() { 
	for (int i = 0; i < NUM_BUTTONS; i++) {
		delete buttons[i];
	}
}

void MiniMenu::draw(float dt) {
	
	if (!active) return;

	//Draw Buttons
	for (int i = 0; i < NUM_BUTTONS; i++) {
		buttons[i]->draw(dt);
	}
	
	
	//Draw mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

void MiniMenu::update(float dt) {

	if (!active) return;

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Update buttons - determine whether or not they are being moused over
	for (int i = 0; i < NUM_BUTTONS; i++) {		
		buttons[i]->update(mouseX, mouseY);
	}

	//Quit (Return to main menu)
	if (buttons[QUIT_BUTTON]->isClicked()) {
		mode = MENU_MODE;
		menu->currentScreen = TITLE_SCREEN;
		active = false;
	}

	//Continue
	if (buttons[CONTINUE_BUTTON]->isClicked()) {
		active = false;
	}


	//Listen for ESC to close mini menu and return to main menu
	if (hge->Input_KeyDown(HGEK_ESCAPE)) {
		mode = MENU_MODE;
		menu->currentScreen = TITLE_SCREEN;
		active = false;
	}

}
