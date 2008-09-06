#include "Game.h"
#include "CustomizeScreen.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Menu *menu;
extern GameInfo gameInfo;
extern bool menuMusicPlaying;
extern int mode;
extern Song songs[NUM_SONGS];
extern hgeAnimation *itemAnimations[NUM_ITEMS-1], *botonoidGraphics[NUM_BOTONOIDS];
extern Player *players[3];

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

	//Handicap bars
	float yOffset = 117.0;
	int player = 0;
	char gay[10];
	for (int i = 0; i < 6; i++) {
		
		handicapBars[i].textX = guiX + 102.0f;
		handicapBars[i].textY = guiY + 148.0f + yOffset*player;
		handicapBars[i].mouseOver = false;

		//Walls
		if (i % 2 == 0) {

			strcpy(handicapBars[i].tooltipText, "Points Per Wall");
			handicapBars[i].collisionBox = new hgeRect(
					guiX + 114.0f, 
					guiY + 176.0f + yOffset*player, 
					guiX + 230.0f, 
					guiY + 212.0f + yOffset*player);
			handicapBars[i].bar = new hgeRect(
					guiX + 149.0f, 
					guiY + 176.0f + yOffset*player, 
					guiX + 230.0f, 
					guiY + 207.0f + yOffset*player);
			strcpy(gay, "pointsPerWall");
			strcat(gay, intToString(player));
			gameInfo.pointsPerWall[player] = hge->Ini_GetInt("Options", gay, 1);
		
		//Gardens
		} else {

			strcpy(handicapBars[i].tooltipText, "Points Per Garden");
			handicapBars[i].collisionBox = new hgeRect(
					guiX + 114.0f, 
					guiY + 211.0f + yOffset*player, 
					guiX + 230.0f, 
					guiY + 247.0f + yOffset*player);
			handicapBars[i].bar = new hgeRect(
					guiX + 149.0f, 
					guiY + 216.0f + yOffset*player, 
					guiX + 230.0f, 
					guiY + 247.0f + yOffset*player);
			strcpy(gay, "pointsPerGarden");
			strcat(gay, intToString(player));
			gameInfo.pointsPerGarden[player] = hge->Ini_GetInt("Options", gay, 2);

			player++;
		}

	}

	//Board size collision boxes
	for (int i = 0; i < 4; i++) {
		boardSizeBoxes[i] = new hgeRect(guiX+567.0 + i*80.0, guiY+37.0, guiX+567.0+77.0+i*80.0, guiY+37.0+35.0);
	}

	//Item frequency bars
	for (int i = 0; i < NUM_ITEMS-1; i++) {
		int x = (i < 4 ? 326 : 644);
		int y = 242 + 80*(i%4);
		itemBars[i] = new hgeRect(x, y, x+112, y + 40);
		itemBarClicked[i] = false;
	}
	loadItemFrequencies();
	
}

/**
 * Destructor.
 */
CustomizeScreen::~CustomizeScreen() {
	for (int i = 0; i < 2; i++) delete buttons[i];
	for (int i = 0; i < 3; i++) {
		delete handicapBars[i].collisionBox;
		delete handicapBars[i].bar;
	}
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

	//Draw handicap areas
	for (int i = 0; i < gameInfo.numPlayers; i++) {

		//Spinning botonoid
		botonoidGraphics[i]->SetSpeed(14);
		botonoidGraphics[i]->Update(dt);
		botonoidGraphics[i]->Render(113.0, 315.0 + 120.0 * (float)i);

		//Non-tool tip text
		resources->GetFont("tooltip")->printf(handicapBars[i*2].textX, handicapBars[i*2].textY, HGETEXT_LEFT, 
			"Player %d Handicap", i+1);

	}

	//Draw handicap bars
	int player;
	for (int i = 0; i < gameInfo.numPlayers*2; i++) {
		
		player = (i-i%2)/2;

		//Tooltip text
		if (handicapBars[i].mouseOver || handicapBars[i].clicked) {
			resources->GetSprite("blackbox")->RenderStretch(handicapBars[i].textX, handicapBars[i].textY, handicapBars[i].textX + 150.0f, handicapBars[i].textY + 25.0f);
			resources->GetFont("tooltip")->printf(handicapBars[i].textX, handicapBars[i].textY, HGETEXT_LEFT, handicapBars[i].tooltipText);
		}

		//Handicap bars
		for (int j = 5; j > 0; j--) {
			if (i % 2 == 0 && gameInfo.pointsPerWall[player] < j ||
					i % 2 == 1 && gameInfo.pointsPerGarden[player] < j) {
				resources->GetSprite("blackbox")->RenderStretch(
						handicapBars[i].bar->x1 + (j-1) * (82/5) + 2, 
						handicapBars[i].bar->y1 + 2,
						handicapBars[i].bar->x1 + (j-1) * (82/5) + 82/5,
						handicapBars[i].bar->y1 + 1 + 29);
			}
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
	for (int i = 0; i < NUM_ITEMS-1; i++) {
		//Draw black boxes over the squares that aren't be filled
		for (int j = 5; j > 0; j--) {
			if (gameInfo.itemFrequencies[i] < j) {
				resources->GetSprite("blackbox")->RenderStretch(itemBars[i]->x1 + (j-1) * (112/5) + 2, 
																itemBars[i]->y1 + 1,
																itemBars[i]->x1 + (j-1) * (112/5) + 112/5,
																itemBars[i]->y1 + 1 + 42);
			}
		}
		//Spinning icon
		itemAnimations[i]->Update(dt);
		itemAnimations[i]->Render(itemBars[i]->x2 + 21, itemBars[i]->y1 + 21);
	}

	//Black out the handicap area for the 3rd player if there are only 2 players!
	if (gameInfo.numPlayers < 3) {
		resources->GetSprite("blackbox")->RenderStretch(75,455, 305,565);
	}

}

/**
 * Update the customize screen.
 */
bool CustomizeScreen::update(float dt, float _mouseX, float _mouseY) {

	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Determine whether each button is highlighted
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Update handicap bars
	int player;
	for (int i = 0; i < gameInfo.numPlayers*2; i++) {
		player = (i - i%2)/2;
		handicapBars[i].mouseOver = handicapBars[i].collisionBox->TestPoint(mouseX, mouseY);
		if (handicapBars[i].bar->TestPoint(mouseX, mouseY) && hge->Input_KeyDown(HGEK_LBUTTON))
			handicapBars[i].clicked = true;
		if (!hge->Input_GetKeyState(HGEK_LBUTTON)) 
			handicapBars[i].clicked = false;
		if (handicapBars[i].clicked) {
			if (i % 2 == 0) {
				gameInfo.pointsPerWall[player] = (mouseX - handicapBars[i].bar->x1 + 12) / (82/5);
				if (gameInfo.pointsPerWall[player] < 0) gameInfo.pointsPerWall[player] = 0;
				if (gameInfo.pointsPerWall[player] > 5) gameInfo.pointsPerWall[player] = 5;
			} else {
				gameInfo.pointsPerGarden[player] = (mouseX - handicapBars[i].bar->x1 + 12) / (82/5);
				if (gameInfo.pointsPerGarden[player] < 0) gameInfo.pointsPerGarden[player] = 0;
				if (gameInfo.pointsPerGarden[player] > 5) gameInfo.pointsPerGarden[player] = 5;
			}
			saveHandicaps();
		}
	}

	//Select board size
	for (int i = 0; i < 4; i++) {
		if (hge->Input_KeyDown(HGEK_LBUTTON) && boardSizeBoxes[i]->TestPoint(mouseX, mouseY)) {
			gameInfo.boardSize = i;
		}
	}

	//Click on item frequency bars
	for (int i = 0; i < NUM_ITEMS-1; i++) {

		if (itemBars[i]->TestPoint(mouseX, mouseY) && hge->Input_KeyDown(HGEK_LBUTTON))
			itemBarClicked[i] = true;
		if (!hge->Input_GetKeyState(HGEK_LBUTTON)) 
			itemBarClicked[i] = false;
		if (itemBarClicked[i]) {
			gameInfo.itemFrequencies[i] = (mouseX - itemBars[i]->x1 + 12) / (112/5);
			if (gameInfo.itemFrequencies[i] < 0) gameInfo.itemFrequencies[i] = 0;
			if (gameInfo.itemFrequencies[i] > 5) gameInfo.itemFrequencies[i] = 5;
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


/**
 * Saves currently set handicaps to the INI file
 */
void CustomizeScreen::saveHandicaps() {
	std::string param;
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		//Wall handicaps
		param = "pointsPerWall";
		param += intToString(i);
		hge->Ini_SetInt("Options", param.c_str(), gameInfo.pointsPerWall[i]);
		//Garden handicaps
		param = "pointsPerGarden";
		param += intToString(i);
		hge->Ini_SetInt("Options", param.c_str(), gameInfo.pointsPerGarden[i]);
	}
}
