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
	addButton(QUIT_BUTTON, "Quit");
	addButton(CONTINUE_BUTTON, "Continue");

}

/**
 * Destructor.
 */
MiniMenu::~MiniMenu() { 
	for (int i = 0; i < NUM_BUTTONS; i++) {
		delete buttons[i].collisionBox;
	}
}

void MiniMenu::draw(float dt) {
	
	if (!active) return;

	//Draw Buttons
	for (int i = 0; i < NUM_BUTTONS; i++) {

		//Button background
		if (buttons[i].highlighted) {
			resources->GetSprite("miniMenuButtonHighlighted")->Render(buttons[i].x, buttons[i].y);
		} else {
			resources->GetSprite("miniMenuButton")->Render(buttons[i].x, buttons[i].y);
		}

		//Button text
		resources->GetFont("timer")->SetScale(1.5f);
		resources->GetFont("timer")->SetColor(ARGB(255,0,0,0));
		resources->GetFont("timer")->printf(buttons[i].x + BUTTON_WIDTH/2, buttons[i].y+15.0f, HGETEXT_CENTER, buttons[i].text);
		resources->GetFont("timer")->SetScale(1.0f);
		resources->GetFont("timer")->SetColor(ARGB(255,255,255,255));

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
		buttons[i].highlighted = buttons[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Listen for mouse click
	if (hge->Input_KeyDown(HGEK_LBUTTON)) {

		//Quit (Return to main menu)
		if (buttons[QUIT_BUTTON].highlighted) {
			mode = MENU_MODE;
			menu->currentScreen = TITLE_SCREEN;
			active = false;
		}

		//Continue
		if (buttons[CONTINUE_BUTTON].highlighted) {
			active = false;
		}

	}

	//Listen for ESC to close mini menu and return to main menu
	if (hge->Input_KeyDown(HGEK_ESCAPE)) {
		mode = MENU_MODE;
		active = false;
	}

}

void MiniMenu::addButton(int num, char* _text) {

	strcpy(buttons[num].text, _text);
	buttons[num].x = (1024 - BUTTON_WIDTH) / 2;
	buttons[num].y = (768 - NUM_BUTTONS*BUTTON_HEIGHT + (NUM_BUTTONS-1)*25) / 2 + BUTTON_HEIGHT*num + 25*(num-1);
	buttons[num].highlighted = false;
	buttons[num].collisionBox = new hgeRect(buttons[num].x, buttons[num].y, buttons[num].x + BUTTON_WIDTH, buttons[num].y + BUTTON_HEIGHT);

}
