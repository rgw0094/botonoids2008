#include "game.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern Input *input;
extern GameInfo gameInfo;
extern int mode;
extern HCHANNEL musicChannel;

#define CANCEL_BUTTON 0
#define DEFAULTS_BUTTON 1

/**
 * Constructor.
 */
OptionsScreen::OptionsScreen() {

	//Create Buttons
	buttons[CANCEL_BUTTON] = new Button(100.0f, 650.0f, "done");
	buttons[DEFAULTS_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "defaults");

	//Coordinates of the top left pixel of the customize window
	guiX = (1024.0f-927.0f)/2.0f;
	guiY = (768.0f-511.0f)/2.0f - 50.0f;

	inputBox = new hgeRect();
	musicVolumeBox = new hgeRect(752.0, 152.0, 813.0, 514.0);
	soundVolumeBox = new hgeRect(832.0, 152.0, 893.0, 514.0);

	

}	

/**
 * Destructor.
 */
OptionsScreen::~OptionsScreen() {
	for (int i = 0; i < 2; i++) {
		delete buttons[i];
	}
	delete inputBox;
	delete soundVolumeBox;
	delete musicVolumeBox;
}

/**
 * Draw the botonoid select screen.
 */
void OptionsScreen::draw(float dt) {

	//Draw background
	resources->GetSprite("titlescreen")->Render(0,0);
	resources->GetSprite("optionsGUI")->Render(guiX, guiY);

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

	//Controls
	for (int player = 0; player < 3; player++) {
		for (int control = 0; control < NUM_INPUTS; control++) {
			
			float x = 280.0 + player*154.0;
			float y = 190.0 + control*45.0;
			
			//If the control is in edit mode
			if (input->inputs[player][control].editMode) {
				resources->GetFont("input")->SetColor(ARGB(255,255,255,255));
				resources->GetFont("input")->printf(x, y, HGETEXT_CENTER, "Press Button");

			//If the control is using the keyboard
			} else if (input->inputs[player][control].keyboard) {
				resources->GetFont("input")->SetColor(ARGB(255,200,200,200));
				resources->GetFont("input")->printf(x, y, HGETEXT_CENTER, hge->Input_GetKeyName(input->inputs[player][control].code));
			
			//If the control is using the gamepad
			} else {
				resources->GetFont("input")->SetColor(ARGB(255,20,20,255));
				resources->GetFont("input")->printf(x, y, HGETEXT_CENTER, "Button %d", input->inputs[player][control].code);
			}

		}
	}

	//Draw volume bars
	for (int i = 100; i > 0; i -= 10) {
		//Music volume
		if (gameInfo.musicVolume >= (i - 10)) {
			resources->GetSprite("volumeBlock")->Render(musicVolumeBox->x1 + 1.0, 153.0 + 36.0*(10-i/10));
		}
		//Sound volume
		if (gameInfo.soundVolume >= (i - 10)) {
			resources->GetSprite("volumeBlock")->Render(soundVolumeBox->x1 + 1.0, 153.0 + 36.0*(10-i/10));
		}
	}

}

/**
 * Update the botonoid select screen.
 */
bool OptionsScreen::update(float dt, float mouseX, float mouseY) {

	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Update buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Click on music volume bar
	if (musicVolumeBox->TestPoint(mouseX, mouseY) && hge->Input_GetKeyState(HGEK_LBUTTON)) {
		gameInfo.musicVolume = ((514.0 - mouseY - 25) / 36) * 10;
		hge->Channel_SetVolume(musicChannel, gameInfo.musicVolume);
		hge->Ini_SetInt("Options", "musicVolume", gameInfo.musicVolume);
	}

	//Click on sound volume bar
	if (soundVolumeBox->TestPoint(mouseX, mouseY) && hge->Input_GetKeyState(HGEK_LBUTTON)) {
		gameInfo.soundVolume = ((514.0 - mouseY - 25) / 36) * 10;
		hge->Ini_SetInt("Options", "soundVolume", gameInfo.soundVolume);
	}
	
	//Click Cancel Button
	if (buttons[CANCEL_BUTTON]->isClicked()) {
		menu->currentScreen = TITLE_SCREEN;
		//Turn edit mode off for all inputs
		input->setEditMode(-1,-1);
	}

	//Click Defaults Button
	if (buttons[DEFAULTS_BUTTON]->isClicked()) {
		//Restore defaults
		//...
	}

	//Controls
	for (int player = 0; player < 3; player++) {
		for (int control = 0; control < NUM_INPUTS; control++) {
			
			//Set collision box for this control
			float x = 280.0 + player*154.0;
			float y = 190.0 + control*45.0;
			inputBox->Set(x - 75.0, y - 5.0, x + 75.0, y + 35.0);
			drawCollisionBox(inputBox, 255, 255, 255);
			
			//Click on the control box
			if (hge->Input_KeyDown(HGEK_LBUTTON) && inputBox->TestPoint(mouseX, mouseY)) {
				input->setEditMode(player, control);
			}

			//If the control is in edit mode, listen for the new input
			if (input->inputs[player][control].editMode) {

				input->listenForNewInput(player, control);

			}

		}
	}

	return false;
}