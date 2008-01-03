#include "Game.h"
#include "CustomizeScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern GameInfo gameInfo;
extern bool menuMusicPlaying;
extern int mode;
extern Song songs[NUM_SONGS];
extern hgeAnimation *itemAnimations[10];

#define BACK_BUTTON 0
#define NEXT_BUTTON 1

/**
 * Constructor.
 */
CustomizeScreen::CustomizeScreen() {
	
	selectedSong = 0;

	//Song selection GUI items
	nextSongBox = new hgeRect(265.0, 175.0, 295.0, 205.0);
	previousSongBox = new hgeRect(88.0, 175.0, 115.0, 205.0);
	playSongBox = new hgeRect(120.0, 175.0, 260.0, 205.0);

	//Create buttons
	buttons[BACK_BUTTON] = new Button(100.0f, 650.0f, "back");
	buttons[NEXT_BUTTON] = new Button(1024.0f - 100.0f - BUTTON_WIDTH, 650.0f, "play");

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

	//Board size collision boxes
	for (int i = 0; i < 4; i++) {
		boardSizeBoxes[i] = new hgeRect(guiX+567.0 + i*80.0, guiY+37.0, guiX+567.0+77.0+i*80.0, guiY+37.0+35.0);
	}

	//Item frequency bars
	for (int i = 0; i < 10; i++) {
		int x = (i < 5 ? 321 : 639);
		itemBars[i] = new hgeRect(x, 223 + 60*(i%5), x+112, 268 + 60*(i%5));
	}
	


}

/**
 * Destructor.
 */
CustomizeScreen::~CustomizeScreen() {
	for (int i = 0; i < 2; i++) delete buttons[i];
	for (int i = 0; i < 6; i++) delete tooltips[i].collisionBox;
	for (int i = 0; i < 4; i++) delete boardSizeBoxes[i];
	delete nextSongBox;
	delete previousSongBox;
	delete playSongBox;
}

/**
 * Draw the botonoid select screen.
 */
void CustomizeScreen::draw(float dt) {
	
	//Draw background and GUI
	resources->GetSprite("titlescreen")->Render(0.0f, 0.0f);
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

	//Board size boxes
	for (int i = 0; i < 4; i++) {
		if (gameInfo.boardSize == i) {
			drawCollisionBox(boardSizeBoxes[i], 0, 180, 0);
		} else if (boardSizeBoxes[i]->TestPoint(mouseX, mouseY)) {
			drawCollisionBox(boardSizeBoxes[i], 255, 255, 255);
		}
	}

	//Song selection
	resources->GetFont("timer")->printf(193.0, 138.0, HGETEXT_CENTER, "%s", songs[selectedSong].songName);
	if (playSongBox->TestPoint(mouseX, mouseY)) {
		resources->GetFont("smallBattlefield")->SetColor(ARGB(255, 230, 0, 0));
		resources->GetFont("smallBattlefield")->printf(193.0, 182.0, HGETEXT_CENTER, "Play Song");
	} else {
		resources->GetFont("smallBattlefield")->SetColor(ARGB(255, 255, 255, 255));
		resources->GetFont("smallBattlefield")->printf(193.0, 182.0, HGETEXT_CENTER, "Play Song");
	}

	//Draw item frequency bars
	for (int i = 0; i < 10; i++) {
		//Draw black boxes over the squares that aren't be filled
		for (int j = 5; j > 0; j--) {
			if (gameInfo.itemFrequencies[i] < j) {
				resources->GetSprite("blackbox")->RenderStretch(itemBars[i]->x1 + (j-1) * (112/5) + 1, 
																itemBars[i]->y1 + 1,
																itemBars[i]->x1 + (j-1) * (112/5) + 112/5,
																itemBars[i]->y1 + 1 + 42);
			}
		}
		//Spinning icon
		itemAnimations[i]->Update(dt);
		itemAnimations[i]->Render(itemBars[i]->x2 + 21, itemBars[i]->y1 + 21);
	}

}

/**
 * Update the botonoid select screen.
 */
bool CustomizeScreen::update(float dt, float _mouseX, float _mouseY) {

	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Update tooltips
	for (int i = 0; i < 6; i++) {
		tooltips[i].mouseOver = tooltips[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Select board size
	for (int i = 0; i < 4; i++) {
		if (hge->Input_KeyDown(HGEK_LBUTTON) && boardSizeBoxes[i]->TestPoint(mouseX, mouseY)) {
			gameInfo.boardSize = i;
		}
	}

	//Click on item frequency bars
	for (int i = 0; i < 10; i++) {
		if (itemBars[i]->TestPoint(mouseX, mouseY) && hge->Input_GetKeyState(HGEK_LBUTTON)) {
			gameInfo.itemFrequencies[i] = (mouseX - itemBars[i]->x1 + 12) / (112/5);
			saveItemFrequencies();
		}
	}

	//Click Play Song Button
	if (hge->Input_KeyDown(HGEK_LBUTTON) && playSongBox->TestPoint(mouseX, mouseY)) {
		setMusic(songs[selectedSong].fileName);
	}

	//Click Next Song Button
	if (hge->Input_KeyDown(HGEK_LBUTTON) && nextSongBox->TestPoint(mouseX, mouseY)) {
		selectedSong++;
		if (selectedSong == NUM_SONGS) selectedSong = 0;
	}

	//Click Previous Song Button
	if (hge->Input_KeyDown(HGEK_LBUTTON) && previousSongBox->TestPoint(mouseX, mouseY)) {
		selectedSong--;
		if (selectedSong < 0) selectedSong = NUM_SONGS-1;
	}
	
	//Click Back Button
	if (buttons[BACK_BUTTON]->isClicked()) {
		menu->currentScreen = SELECT_SCREEN;
		if (!menuMusicPlaying) setMusic("menu");
	}

	//Click Start Button
	if (buttons[NEXT_BUTTON]->isClicked()) {
		//Save music choice
		gameInfo.gameMusic = selectedSong;
		//Start the game
		startGame();
	}

	return false;
}