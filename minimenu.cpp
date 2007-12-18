#include "game.h"

extern hgeResourceManager *resources;
extern HGE *hge;
extern int mode;

MiniMenu::MiniMenu() {
	mouseX = mouseY = 400.0f;
	active = false;
}

MiniMenu::~MiniMenu() {

}

void MiniMenu::draw(float dt) {
	
	if (!active) return;

	//Draw mini menu window
	resources->GetSprite("minimenu")->Render(300,300);
	
	//Draw mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

void MiniMenu::update(float dt) {

	if (!active) return;

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Close mini menu and return to main menu
	if (hge->Input_KeyDown(HGEK_ESCAPE)) {
		mode = MENU_MODE;
		active = false;
	}

}