#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include "game.h"

#define NUM_ITEMS 1
#define ITEM_SILLY_PAD 0

struct Item {
	int itemCode;
	int player;
	float x, y, dx, dy, radius;
	hgeParticleSystem *trail;
	hgeRect *collisionBox;
};


class ItemManager {

public:

	ItemManager();
	~ItemManager();

	void draw(float dt);
	void update(float dt);
	void generateItem(int gridX, int gridY, int gardenSize, int whichPlayer);
	void reset();

	std::list<Item> itemList;
	hgeSprite *itemSprites[NUM_ITEMS];
	hgeAnimation *itemAnimations[NUM_ITEMS];

};

#endif