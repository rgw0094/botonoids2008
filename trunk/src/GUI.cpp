#include "GUI.h"
#include "game.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Player *players[3];
extern GameInfo gameInfo;
extern hgeFont *debugFont;
extern float timer;
extern int mode;

/**
 * Constructor.
 */
GUI::GUI() { 
	barSprites[0] = resources->GetSprite("goldBar");
	barSprites[1] = resources->GetSprite("blackBar");
	barSprites[2] = resources->GetSprite("silverBar");
	windowSprites[0] = resources->GetSprite("goldWindow");
	windowSprites[1] = resources->GetSprite("blackWindow");
	windowSprites[2] = resources->GetSprite("silverWindow");
	faceAnimations[0] = resources->GetAnimation("goldFace");
	faceAnimations[1] = resources->GetAnimation("blackFace");
	faceAnimations[2] = resources->GetAnimation("silverFace");
	healthSprites[0] = resources->GetSprite("goldHealth");
	healthSprites[1] = resources->GetSprite("blackHealth");
	healthSprites[2] = resources->GetSprite("silverHealth");

	//temp
	for (int i = 0; i < 3; i++) faceAnimations[i]->Play();
}

GUI::~GUI() { }

void GUI::draw(float dt) {

	//Draw GUI
	resources->GetSprite("guiTop")->Render(860.0, 0.0);
	resources->GetSprite("guiBottom")->Render(860.0, 384.0);

	//Draw the timer
	drawTimer(dt);

	//Calculate score related shit
	int maxScore = 0;
	bool allZero = true;
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (players[i]->score > maxScore) maxScore = players[i]->score;
		if (players[i]->score > 0.0f) allZero = false;
	}

	//Set yOffset that will center the scores vertically
	int yOffset;
	if (gameInfo.numPlayers == 3) yOffset = 154;
	else if (gameInfo.numPlayers == 2) yOffset = 165;

	//Draw each player's shit
	float barLength = 0.0f;
	for (int i = 0; i < gameInfo.numPlayers; i++) {

			//Player scores
			if (allZero) {
				//All players have a full bar when no one has scored yet
				barLength = 76.0f;
			} else {
				barLength = ((float)players[i]->score / (float)maxScore) * 76.0f;
			}
			barSprites[players[i]->whichBotonoid]->RenderStretch(922, yOffset+18*i, 922.0f+barLength, yOffset+18+18*i);
			resources->GetFont("score")->printf(890,yOffset+18*i,HGETEXT_LEFT,"%d", players[i]->score);
	
			//Item windows
			windowSprites[players[i]->whichBotonoid]->Render(885, 288 + (85+98)*i);

			//Face animation
			faceAnimations[players[i]->whichBotonoid]->Update(dt);
			faceAnimations[players[i]->whichBotonoid]->Render(885, 221 + (85+98)*i);

			//Health
			if (players[i]->health >= 1) 
				healthSprites[players[i]->whichBotonoid]->Render(955,220 + (85+98)*i);
			if (players[i]->health >= 2)
				healthSprites[players[i]->whichBotonoid]->Render(970,235 + (85+98)*i);
			if (players[i]->health >= 3)
				healthSprites[players[i]->whichBotonoid]->Render(955,250 + (85+98)*i);

	}

}

/**
 * Updates the GUI
 */
void GUI::update(float dt) {
	
	//Update timer
	timer -= dt;

	if (hge->Input_KeyDown(HGEK_T)) timer = -1.0;
	
	//When the timer reaches 0 the game is over
	if (timer < 0.0) {
		timer = 0.0;
		endGame();
	}

}

/**
 * Draws and updates the timer
 */
void GUI::drawTimer(float dt) {

	char gay[10];
        
	//Format time string - wow I wish C had a decent String class!
    int time = (int)timer;
    int minutes = (time - (time % 60)) / 60;
    time -= minutes*60; 
	std::string timeString = "";
	timeString += itoa(minutes, gay, 10);
	timeString += ":";	
	std::string secondsString = "";
	if (time < 10) secondsString += "0";
	secondsString += itoa(time, gay, 10);
	timeString += secondsString.c_str();

	//Output time string
	resources->GetFont("timer")->printf(940,28,HGETEXT_CENTER, "%s", timeString.c_str());

}