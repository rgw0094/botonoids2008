#ifndef _STATSPAGE_H_
#define _STATSPAGE_H_

#include "game.h"
#include "button.h"

#define NUM_STATS 7
#define WALL_ICON 0
#define GARDEN_ICON 1
#define BIGGEST_COMBO_ICON 2
#define TIME_IN_FIRST_ICON 3
#define ITEMS_USED_ICON 4
#define DAMAGE_DEALT_ICON 5
#define DAMAGE_TAKEN_ICON 6

struct PlayerStats {
	int wallsBuilt;
	int gardensBuilt;
	int biggestCombo;
	float timeWinning;
	int numItemsUsed;
	int damageDealt;
	int damageTaken;
};

struct Icon {
	hgeSprite *graphic;
	hgeRect *collisionBox;
	char tooltip[50];
	bool mouseOver;
	float x,y;
};


class StatsPage {

public:

	StatsPage();
	~StatsPage();

	void draw(float dt);
	void update(float dt);
	void placeIcon(int whichIcon, float x, float y);

	bool active;
	PlayerStats stats[3];
	Icon icons[NUM_STATS+3];
	float mouseX, mouseY;
	Button *okButton;

};

#endif