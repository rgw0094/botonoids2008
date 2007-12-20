#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include "game.h"

#define NUM_ITEMS 4

struct Item {
	int itemCode;
	float x, y, dx, dy;
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