#include "Game.h"
#include "CustomizeScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern int mode;

#define BACK_BUTTON 0
#define NEXT_BUTTON 1

/**
 * Constructor.
 */
CustomizeScreen::CustomizeScreen() {

	//Create buttons
	buttons[BACK_BUTTON] = new Button(100.0f, 650.0f, "Return");
	buttons[NEXT_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "Start Game");

	//Coordinates of the top left pixel of the customize window
	guiX = (1024.0f-927.0f)/2.0f;
	guiY = (768.0f-511.0f)/2.0f - 50.0f;

	//Wall handicap tooltips
	int count = 0;
	float yOffset = 118.0f;
	for (int i = 0; i < 6; i += 2) {
		strcpy(tooltips[i].text, "Points Per Wall");
		tooltips[i].collisionBox = new hgeRect(guiX + 100.0f, guiY + 176.0f + yOffset*count, guiX + 216.0f, guiY + 212.0f + yOffset*count);
		tooltips[i].textX = guiX + 102.0f;
		tooltips[i].textY = guiY + 148.0f + yOffset*count;
		tooltips[i].mouseOver = false;
		count++;
	}

	//Garden handicap tooltips
	count = 0;
	for (int i = 1; i < 6; i += 2) {
		strcpy(tooltips[i].text, "Points Per Garden");
		tooltips[i].collisionBox = new hgeRect(guiX + 100.0f , guiY + 212.0f + yOffset*count, guiX + 216.0f, guiY + 247.0f + yOffset*count);
		tooltips[i].textX = guiX + 102.0f;
		tooltips[i].textY = guiY + 148.0f + yOffset*count;
		tooltips[i].mouseOver = false;
		count ++;
	}

}

/**
 * Destructor.
 */
CustomizeScreen::~CustomizeScreen() {
	for (int i = 0; i < 2; i++) delete buttons[i];
	for (int i = 0; i < 6; i++) delete tooltips[i].collisionBox;
}

/**
 * Draw the botonoid select screen.
 */
void CustomizeScreen::draw(float dt) {

	resources->GetSprite("titlescreen")->Render(0.0f, 0.0f);
	resources->GetFont("button")->printf(512.0f, 25.0f, HGETEXT_CENTER, "Customize Game");
	
	//Draw GUI
	resources->GetSprite("customizeGUI")->Render(guiX, guiY);

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

	//Draw tooltips
	for (int i = 0; i < 6; i++) {
		//Non-tool tip text
		if (i % 2 == 0) {
			resources->GetFont("tooltip")->printf(tooltips[i].textX, tooltips[i].textY, HGETEXT_LEFT, "Player %d Handicap", (i/2)+1);
		}
		//Tooltip text
		if (tooltips[i].mouseOver) {
			resources->GetSprite("blackbox")->RenderStretch(tooltips[i].textX, 
															tooltips[i].textY, 
															tooltips[i].textX + 150.0f,
															tooltips[i].textY + 25.0f);
			resources->GetFont("tooltip")->printf(tooltips[i].textX, tooltips[i].textY, HGETEXT_LEFT, tooltips[i].text);
		}
	}

}

/**
 * Update the botonoid select screen.
 */
bool CustomizeScreen::update(float dt, float mouseX, float mouseY) {

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Update tooltips
	for (int i = 0; i < 6; i++) {
		tooltips[i].mouseOver = tooltips[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		menu->currentScreen = SELECT_SCREEN;
	}

	//Click Next Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		startGame();
	}

	return false;
}