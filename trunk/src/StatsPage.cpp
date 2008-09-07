#include "StatsPage.h"
#include "game.h"

extern HGE *hge;
extern GameInfo gameInfo;
extern hgeResourceManager *resources;
extern hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
extern int mode;
extern Menu *menu;
extern std::string botonoidNames[3];
extern Player *players[3];

/**
 * Constructor
 */
StatsPage::StatsPage() {

	active = false;
	mouseX = mouseY = 400.0f;	

	//Wall icon
	icons[WALL_ICON].graphic = resources->GetSprite("wallIcon");
	strcpy(icons[WALL_ICON].tooltip, "Walls Built");
	//Garden icon
	icons[GARDEN_ICON].graphic = resources->GetSprite("gardenIcon");
	strcpy(icons[GARDEN_ICON].tooltip, "Gardens Built");
	//Biggest Combo icon
	icons[BIGGEST_COMBO_ICON].graphic = resources->GetSprite("biggestComboIcon");
	strcpy(icons[BIGGEST_COMBO_ICON].tooltip, "Largest Combo");
	//Time In First icon
	icons[TIME_IN_FIRST_ICON].graphic = resources->GetSprite("timeInFirstIcon");
	strcpy(icons[TIME_IN_FIRST_ICON].tooltip, "Time in First");
	//Items Used icon
	icons[ITEMS_USED_ICON].graphic = resources->GetSprite("itemsUsedIcon");
	strcpy(icons[ITEMS_USED_ICON].tooltip, "Items Used");
	//Damage Dealt icon
	icons[DAMAGE_DEALT_ICON].graphic = resources->GetSprite("damageDealtIcon");
	strcpy(icons[DAMAGE_DEALT_ICON].tooltip, "Damage Dealt");
	//Damage Taken icon
	icons[DAMAGE_TAKEN_ICON].graphic = resources->GetSprite("damageTakenIcon");
	strcpy(icons[DAMAGE_TAKEN_ICON].tooltip, "Damage Taken");

	//Set icon mouseovers to false
	for (int i = 0; i < NUM_STATS; i++) {
		icons[i].mouseOver = false;
	}

	//Init stats
	for (int i = 0; i < 3; i++) {
		stats[i].wallsBuilt = stats[i].gardensBuilt = stats[i].biggestCombo = stats[i].numItemsUsed =
			stats[i].damageDealt = stats[i].damageTaken = 0;
		stats[i].timeWinning = 0.0f;
	
	}

	//Create OK Button
	okButton = new Button(512.0f - BUTTON_WIDTH/2.0f - 67.0f, 575.0f, "Done");

}

/**
 * Destructor
 */
StatsPage::~StatsPage() { 
	delete okButton;
}

/**
 * Draw the stats page
 */
void StatsPage::draw(float dt) {

	if (!active) return;

	float x = 300.0f;
	float y = 100.0f;

	hgeFont *f = resources->GetFont("timer");

	//Draw window
	resources->GetSprite("statsPage")->RenderEx(x - 10.0, y - 15.0 + 630.0, -PI/2.0);

	//Draw winner
	if (gameInfo.winner == -1) {
		//Tie
		f->printf(x + 280.0/2.0 + 5.0, y + 10.0f, HGETEXT_CENTER, "Tie Game!");
	} else {
		f->printf(x + 280.0/2.0 + 5.0, y + 10.0f, HGETEXT_CENTER, "%s Wins!", botonoidNames[players[gameInfo.winner]->whichBotonoid].c_str());
	}

	//Draw Icons
	for (int i = 0; i < NUM_STATS+gameInfo.numPlayers; i++) {

		//Statistic icons
		if (i < NUM_STATS) {

			placeIcon(i, x + 15.0f, y + 110.0f + i*50.0f);
			icons[i].graphic->Render(icons[i].x, icons[i].y);

			if (icons[i].mouseOver) {
				f->printf(icons[i].x + 40.0f, icons[i].y-25.0f, HGETEXT_LEFT, "%s", icons[i].tooltip);
			}

		//Botonoid icons
		} else {

			placeIcon(i, x-16 + 100.0f + (i-NUM_STATS)*75.0f, y-16 + 70.0f);
			int oldFrame = botonoidGraphics[i-NUM_STATS]->GetFrame();
			botonoidGraphics[players[i-NUM_STATS]->whichBotonoid]->SetFrame(0);
			botonoidGraphics[players[i-NUM_STATS]->whichBotonoid]->Render(icons[i].x+16, icons[i].y+16);
			botonoidGraphics[players[i-NUM_STATS]->whichBotonoid]->SetFrame(oldFrame);

			if (icons[i].mouseOver) {
				f->printf(x + 140.0f, y + 87.0f, HGETEXT_CENTER, "%s", botonoidNames[players[i-NUM_STATS]->whichBotonoid].c_str());
			}

		}

	}

	//Output stats
	for (int player = 0; player < gameInfo.numPlayers; player++) {

		//Walls built
		if (stats[player].wallsBuilt == maxInt(stats[0].wallsBuilt, stats[1].wallsBuilt, stats[2].wallsBuilt))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[WALL_ICON].y, HGETEXT_CENTER, "%d", stats[player].wallsBuilt);

		//Gardens built
		if (stats[player].gardensBuilt == maxInt(stats[0].gardensBuilt, stats[1].gardensBuilt, stats[2].gardensBuilt))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[GARDEN_ICON].y, HGETEXT_CENTER, "%d", stats[player].gardensBuilt);

		//Biggest Combo
		if (stats[player].biggestCombo == maxInt(stats[0].biggestCombo, stats[1].biggestCombo, stats[2].biggestCombo))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[BIGGEST_COMBO_ICON].y, HGETEXT_CENTER, "%d", stats[player].biggestCombo);

		//Time in first
		if ((int)stats[player].timeWinning == maxInt((int)stats[0].timeWinning, (int)stats[1].timeWinning, (int)stats[2].timeWinning))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[TIME_IN_FIRST_ICON].y, HGETEXT_CENTER, "%s", formatTime((int)stats[player].timeWinning).c_str());
		
		//Items used
		if (stats[player].numItemsUsed == maxInt(stats[0].numItemsUsed, stats[1].numItemsUsed, stats[2].numItemsUsed))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f,icons[ITEMS_USED_ICON].y, HGETEXT_CENTER, "%d", (int)stats[player].numItemsUsed);
		
		//Damage Dealt
		if (stats[player].damageDealt == maxInt(stats[0].damageDealt, stats[1].damageDealt, stats[2].damageDealt))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[DAMAGE_DEALT_ICON].y, HGETEXT_CENTER, "%d", (int)stats[player].damageDealt);
		
		//Damage Taken
		if (stats[player].damageTaken == minInt(stats[0].damageTaken, stats[1].damageTaken, stats[2].damageTaken))
			f->SetColor(ARGB(255,0,255,0));
		else f->SetColor(ARGB(255,255,0,0));
		f->printf(x + 100.0f + player*75.0f, icons[DAMAGE_TAKEN_ICON].y, HGETEXT_CENTER, "%d", (int)stats[player].damageTaken);

	}
	f->SetColor(ARGB(255,255,255,255));

	//Draw OK Button
	okButton->draw(dt);

	//Draw mouse
	resources->GetSprite("mouse")->Render(mouseX, mouseY);

}

/**
 * Places the specified icon at (_x,_y).
 */
void StatsPage::placeIcon(int whichIcon, float _x, float _y) {
	icons[whichIcon].x = _x;
	icons[whichIcon].y = _y;
	icons[whichIcon].collisionBox = new hgeRect(_x, _y, _x + 32.0f, _y + 32.0f);
}

/**
 * Update the stats page
 */
void StatsPage::update(float dt) {

	//Update mouse position
	hge->Input_GetMousePos(&mouseX, &mouseY);

	//Update icon mouseovers
	for (int i = 0; i < NUM_STATS+gameInfo.numPlayers; i++) {
		icons[i].mouseOver = icons[i].collisionBox->TestPoint(mouseX, mouseY);
	}

	//Mouse click
	okButton->update(mouseX, mouseY);
	if (okButton->isClicked()) {
		active = false;
		menu->returnToMenu();
	}

	//ESC to exit
	if (hge->Input_KeyDown(HGEK_ESCAPE)) {
		mode = MENU_MODE;
		menu->currentScreen = TITLE_SCREEN;
		active = false;
	}

}