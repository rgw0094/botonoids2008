#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include "game.h"

#define NUM_ITEMS 10
#define ITEM_SILLY_PAD 0

struct Item {
	int itemCode;
	float x, y, dx, dy, radius;
	hgeParticleSystem *trail;
	hgeRect *collisionBox;
	hgeAnimation *animation;
};


class ItemManager {

public:

	ItemManager();
	~ItemManager();

	void draw(float dt);
	void update(float dt);
	void generateItem(int gridX, int gridY, int gardenSize);
	void reset();

	std::list<Item> itemList;
	hgeSprite *itemSprites[NUM_ITEMS];

};

#endif