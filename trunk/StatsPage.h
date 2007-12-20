#ifndef _STATSPAGE_H_
#define _STATSPAGE_H_

#include "game.h"

#define NUM_STATS 7
#define WALL_ICON 0
#define GARDEN_ICON 1
#define MAX_SCORE_ICON 2
#define TIME_IN_FIRST_ICON 3
#define ITEMS_USED_ICON 4
#define DAMAGE_DEALT_ICON 5
#define DAMAGE_TAKEN_ICON 6

struct PlayerStats {
	int wallsBuilt;
	int gardensBuilt;
	int maxScore;
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

struct StatsButton {
	float x, float y;
	hgeRect *collisionBox;
	char text[50];
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
	Icon icons[NUM_STATS];
	float mouseX, mouseY;
	StatsButton okButton;

};

#endif